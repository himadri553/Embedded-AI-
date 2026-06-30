/*
  Himadri Saha

  Contains functions to sample audio from mic
  Code based off of: https://github.com/ACANETS/eece-4862-5862-labs/blob/main/lab-2/starter/k1-audio-capture/k1-audio-capture.ino

  
*/
#include "config.h"

// ISR-style callback: PDM calls this when a buffer is ready.
void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;  // 2 bytes per 16-bit sample
}