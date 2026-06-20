#include <Arduino_HS300x.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/micro_mutable_op_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include "model.h"

constexpr int kWindowLen = 6;
constexpr int kFeaturesPerStep = 2;
constexpr int kInputLen = kWindowLen * kFeaturesPerStep;

constexpr unsigned long kSampleIntervalMs = 2000UL;

// Replace these with notebook constants if yours are different
float normTemp(float t) {
  return (t - 8.736408f) / 11.688525f;
}

float normHumidity(float h) {
  return (h - 67.333000f) / 16.830614f;
}

namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kArenaSize = 8 * 1024;
alignas(16) uint8_t tensor_arena[kArenaSize];

float window[kInputLen];
int filled = 0;
unsigned long lastSample = 0;

unsigned long latencyMin = 4294967295UL;
unsigned long latencyMax = 0;
unsigned long latencySum = 0;
int latencyCount = 0;

void pushReading(float nt, float nh) {
  for (int i = 0; i < (kWindowLen - 1) * kFeaturesPerStep; i++) {
    window[i] = window[i + kFeaturesPerStep];
  }

  window[kInputLen - 2] = nt;
  window[kInputLen - 1] = nh;

  if (filled < kWindowLen) filled++;
}

int8_t quantize(float x) {
  int32_t q = lroundf(x / input->params.scale) + input->params.zero_point;
  if (q < -128) q = -128;
  if (q > 127) q = 127;
  return (int8_t)q;
}
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize HS300x sensor!");
    while (1) { ; }
  }

  pinMode(LED_BUILTIN, OUTPUT);

  model = tflite::GetModel(g_model);

  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema version mismatch!");
    while (1) { ; }
  }
static tflite::MicroMutableOpResolver<2> resolver;
resolver.AddFullyConnected();
resolver.AddRelu();

  static tflite::MicroInterpreter static_interpreter(
    model,
    resolver,
    tensor_arena,
    kArenaSize
  );

  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("AllocateTensors() failed. Increase kArenaSize.");
    while (1) { ; }
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("Weather inference ready.");
}

void loop() {
  unsigned long now = millis();

  if (now - lastSample < kSampleIntervalMs) {
    return;
  }

  lastSample = now;

  float t = HS300x.readTemperature();
  float h = HS300x.readHumidity();

  pushReading(normTemp(t), normHumidity(h));

  if (filled < kWindowLen) {
    Serial.print("filling window ");
    Serial.print(filled);
    Serial.print("/");
    Serial.println(kWindowLen);
    return;
  }

  for (int i = 0; i < kInputLen; i++) {
    input->data.int8[i] = quantize(window[i]);
  }

  unsigned long startTime = micros();

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Invoke failed");
    return;
  }

  unsigned long elapsed = micros() - startTime;

  latencyCount++;
  latencySum += elapsed;

  if (elapsed < latencyMin) latencyMin = elapsed;
  if (elapsed > latencyMax) latencyMax = elapsed;

  float p = (output->data.int8[0] - output->params.zero_point) * output->params.scale;
  bool snow = p > 0.5f;

  digitalWrite(LED_BUILTIN, snow ? HIGH : LOW);

  Serial.print("temp_c=");
  Serial.print(t);
  Serial.print(", humidity_pct=");
  Serial.print(h);
  Serial.print(", p_snow=");
  Serial.print(p, 4);
  Serial.print(", prediction=");
  Serial.print(snow ? "SNOW LIKELY" : "NO SNOW");
  Serial.print(", latency_us=");
  Serial.println(elapsed);

  if (latencyCount >= 20) {
    Serial.print("[LAT] min=");
    Serial.print(latencyMin);
    Serial.print(" us, mean=");
    Serial.print(latencySum / latencyCount);
    Serial.print(" us, max=");
    Serial.print(latencyMax);
    Serial.println(" us");

    latencyCount = 0;
    latencySum = 0;
    latencyMin = 4294967295UL;
    latencyMax = 0;
  }
}