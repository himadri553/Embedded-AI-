/*
  Himadri Saha

  project_src.ino
    - Is the main entry of this program

*/
#include "config.h"

void setup() {
  // Start terminal and Pin Inits
  Serial.begin(SERIAL_BAUD);
  while (!Serial) { ; }
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_INDICATOR, OUTPUT);

  /* Setup mic */ 
  PDM.onReceive(onPDMdata);
  // PDM.setGain(80);   // uncomment/tune if your levels read too low

  // Handle mic fail
  if (!PDM.begin(PDM_CHANNELS, SAMPLE_RATE)) {
    Serial.println("Failed to start PDM microphone!");
    while (1) { ; }
  }

}

void loop() {
  /* Sample audio from mic */
  // for valid samples, get raw audio levels (peak)
  if (samplesRead == 0) return;
  int16_t peak = 0;
  for (int i = 0; i < samplesRead; i++) {
    int16_t v = abs(sampleBuffer[i]);
    if (v > peak) peak = v;
  }
  Serial.println(peak);
  samplesRead = 0;  // consume the buffer



  /* Later steps */
  // Compute features
  // Run inference
  // Output a decision and drive leds
  // Advance the window
}
