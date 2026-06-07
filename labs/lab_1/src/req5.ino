/*
  Requirement 5

  Open the starter/hello_world/hello_world.ino sketch provided in the lab repo (the
  canonicalTFLMsine-wavedemo,Apache-2.0,mirroredfromGoogle’stflite-micro-arduino-examples).
  Read it top to bottom (~100 lines) and write one-sentence comments in your lab report for:
  the model array, the AllocateTensors() call, the inference loop (Invoke()), and the
  output post-processing
*/

void req5() {
  Serial.println("Model Array: Maps existing g_model byte array to a useable tflite::Model* object");
  Serial.println("AllocateTensors(): Reserves space inside tensor_arena for the model's input/output/intermediate tensors and reports whether they fit via allocate_status.");
  Serial.println("Invoke(): Executes the model's forward computation on the current input tensor and writes its prediction into output, checking invoke_status for errors.");
  Serial.println("Output post-processing: Dequantizes output's int8 value into a float y via its scale/zero_point, then hands (x, y) off to HandleOutput()"):

  delay(10000)
}