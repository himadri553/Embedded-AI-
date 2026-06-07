/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// ===========================================================================
// Lab 1 · B.3 starter — instrument this sketch.
//
// This is the working hello_world sketch (B.1/B.2). Your B.3 task is to
// measure inference latency: wrap each Invoke() with micros(), and over
// at least 100 successive inferences report the MIN, MEAN, and MAX (also
// note the flash/RAM figures from the IDE compile output) — see handout
// §B.3. The two TODO(student) blocks below mark exactly where to add
// code; the rest of the sketch is intentionally unchanged. Build/flash
// it first to confirm it runs, then add your instrumentation.
// ===========================================================================

#include <TensorFlowLite.h>

#include "constants.h"
#include "main_functions.h"
#include "model.h"
#include "output_handler.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 2000;
// Keep aligned to 16 bytes for CMSIS
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

// Rolling-window latency accumulators (B.3): track Invoke() timings
// across the current batch of inferences so min/mean/max can be
// reported once the window fills, then reset for the next batch.
constexpr int kLatencyWindowSize = 100;
int latency_sample_count = 0;
unsigned long latency_sum_us = 0;
unsigned long latency_min_us = 0xFFFFFFFFUL;
unsigned long latency_max_us = 0;
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  tflite::InitializeTarget();

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;
}

// The name of this function is important for Arduino compatibility.
void loop() {
  // Calculate an x value to feed into the model. We compare the current
  // inference_count to the number of inferences per cycle to determine
  // our position within the range of possible x values the model was
  // trained on, and use this to calculate a value.
  float position = static_cast<float>(inference_count) /
                   static_cast<float>(kInferencesPerCycle);
  float x = position * kXrange;

  // Quantize the input from floating-point to integer
  int8_t x_quantized = x / input->params.scale + input->params.zero_point;
  // Place the quantized input in the model's input tensor
  input->data.int8[0] = x_quantized;

  unsigned long invoke_start_us = micros();

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    MicroPrintf("Invoke failed on x: %f\n", static_cast<double>(x));
    return;
  }

  // Accumulate this Invoke() timing into the rolling window. Serial is
  // used (not MicroPrintf) because this TFLM build's MicroPrintf mishandles
  // %lu for the unsigned long that micros() returns.
  unsigned long invoke_elapsed_us = micros() - invoke_start_us;
  latency_sample_count++;
  latency_sum_us += invoke_elapsed_us;
  if (invoke_elapsed_us < latency_min_us) latency_min_us = invoke_elapsed_us;
  if (invoke_elapsed_us > latency_max_us) latency_max_us = invoke_elapsed_us;

  if (latency_sample_count >= kLatencyWindowSize) {
    unsigned long latency_mean_us = latency_sum_us / latency_sample_count;
    Serial.print("[LAT] min=");
    Serial.print(latency_min_us);
    Serial.print(" us, mean=");
    Serial.print(latency_mean_us);
    Serial.print(" us, max=");
    Serial.print(latency_max_us);
    Serial.println(" us");

    latency_sample_count = 0;
    latency_sum_us = 0;
    latency_min_us = 0xFFFFFFFFUL;
    latency_max_us = 0;
  }

  // Obtain the quantized output from model's output tensor
  int8_t y_quantized = output->data.int8[0];
  // Dequantize the output from integer to floating-point
  float y = (y_quantized - output->params.zero_point) * output->params.scale;

  // Output the results. A custom HandleOutput function can be implemented
  // for each supported hardware target.
  HandleOutput(x, y);

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycle
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;
}
