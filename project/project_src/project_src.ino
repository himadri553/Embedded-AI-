/*
  Himadri Saha

  project_src.ino
    - Is the main entry of this program

*/
#include "config.h"

void setup() {
  // Start terminal and Pin Inits
  Serial.begin(SERIAL_BAUD);
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
  // Print raw PCM samples as CSV for EI data forwarder
  if (samplesRead == 0) return;
  for (int i = 0; i < samplesRead; i++) {
    Serial.print(sampleBuffer[i]);
    if (i < samplesRead - 1) Serial.print(",");
  }
  Serial.println(); 
  samplesRead = 0;  


  /* Later steps */
  // Compute features
  // Run inference
  // Output a decision and drive leds
  // Advance the window
}
