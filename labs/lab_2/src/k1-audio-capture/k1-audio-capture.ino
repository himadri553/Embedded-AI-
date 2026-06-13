/*
  // Lab 2 · Track K · A.1 — Microphone capture   (deliverable: k1-audio-capture.ino)
  //
  // Board: Arduino Nano 33 BLE Sense Rev2 (on-board PDM microphone).
  // The PDM library ships with the Arduino Mbed OS Nano core (no install).
  //
  // Goal: capture audio buffers from the mic and report over Serial that
  // capture is working (samples read + a simple level per buffer). This is
  // also the sketch you point the EDGE IMPULSE DATA FORWARDER at to collect
  // labeled keyword samples for B.1.
  //
  // Skeleton compiles/uploads as-is. Fill in each TODO(student).
*/

#include <PDM.h>

// Mono, 16 kHz is the usual KWS configuration (matches Edge Impulse
// audio projects). Buffer holds one chunk of 16-bit PCM samples.
static const int kChannels = 1;
static const int kSampleRate = 16000;
short sampleBuffer[256];
volatile int samplesRead = 0;

// ISR-style callback: PDM calls this when a buffer is ready.
void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;  // 2 bytes per 16-bit sample
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  PDM.onReceive(onPDMdata);
  // The Rev2 mic typically needs some gain; the core's default may be 0.
  // PDM.setGain(80);   // uncomment/tune if your levels read too low

  if (!PDM.begin(kChannels, kSampleRate)) {
    Serial.println("Failed to start PDM microphone!");
    while (1) { ; }
  }
}

void loop() {
  // wait for a fresh buffer
  if (samplesRead == 0) return;  

  /*
  // TODO(student): report that capture works. At minimum print the number
  // of samples in this buffer. Then compute a simple LEVEL (e.g. peak
  // absolute amplitude, or RMS) across sampleBuffer[0..samplesRead) and
  // print it — speak into the mic and watch the level rise.
  */
  
  // Get peak absolute amplitude for level
  int level = 0;
  for (int i = 0; i < samplesRead; i++) {
    int sample = abs(sampleBuffer[i]);
    if (sample > level) level = sample;
  }

  Serial.print(samplesRead); Serial.print(','); Serial.println(level);

  /*
    NOTE: when collecting data for Edge Impulse, you instead stream the
    raw samples in the format the EI data forwarder expects (see the
    k2-kws-inference/README.md and the EI docs). For A.1, the level
    report above is enough.
  */

  // consume the buffer
  samplesRead = 0;  
}