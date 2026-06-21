/*
    Himadri Saha

    audio.ino
        - Audio Capture Layer: reads the onboard PDM microphone (MP34DT06JS)
          on the Arduino Nano 33 BLE Sense Rev2.
        - Capture is interrupt-driven via the PDM onReceive callback so the
          main loop never blocks waiting on the mic.
        - Exposes a small interface to the rest of the pipeline:
            audioBegin()         - initialize the mic
            audioSamplesReady()  - is a fresh chunk available?
            audioReadSamples()   - copy the chunk out and clear the flag
*/

#include <PDM.h>
#include "config.h"

/* Capture buffer filled by the PDM ISR callback */
static int16_t  audioBuffer[AUDIO_CHUNK];
static volatile int audioSamplesRead = 0;  // # of valid int16 samples in audioBuffer

/* ISR-style callback: keep it tiny - just move bytes and flag */
static void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(audioBuffer, bytesAvailable);
  audioSamplesRead = bytesAvailable / 2;   // 2 bytes per int16 sample
}

/* Initialize the PDM mic. Returns true on success. */
bool audioBegin() {
  PDM.onReceive(onPDMdata);
  // PDM.begin(channels, sampleRate)
  if (!PDM.begin(PDM_CHANNELS, SAMPLE_RATE)) {
    return false;
  }
  return true;
}

/* Has a fresh chunk of samples arrived since the last read? */
bool audioSamplesReady() {
  return audioSamplesRead > 0;
}

/*
   Copy the latest chunk into dest (up to maxSamples) and clear the ready flag.
   Returns the number of samples copied.
*/
int audioReadSamples(int16_t* dest, int maxSamples) {
  // Snapshot the count, then clear, with interrupts off so the ISR can't
  // change audioSamplesRead mid-copy.
  noInterrupts();
  int n = audioSamplesRead;
  if (n > maxSamples) n = maxSamples;
  for (int i = 0; i < n; i++) {
    dest[i] = audioBuffer[i];
  }
  audioSamplesRead = 0;
  interrupts();
  return n;
}
