/*
  Himadri Saha

  project_src.ino
    - Is the main entry of this program

*/
#include "config.h"

/* --- Step 1 test scratch: remove once Audio Capture is verified --- */
#define AUDIO_TEST 1
static int16_t  testSamples[AUDIO_CHUNK];
static unsigned long lastRateCheck = 0;
static unsigned long samplesThisSecond = 0;

void setup() {
  // Start terminal
  Serial.begin(SERIAL_BAUD);

  // Pin Inits
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_INDICATOR, OUTPUT);

  // Audio Capture Layer init
  if (!audioBegin()) {
    Serial.println("ERROR: PDM mic failed to start");
    while (1);  // halt - nothing works without the mic
  }
}

void loop() {
  // Get new audio if PCM samples has accumulated. Return if not ready
  if (!audioSamplesReady()) {
    return;
  }
  int n = audioReadSamples(testSamples, AUDIO_CHUNK);

#if AUDIO_TEST
  // Non-blocking check: indicator should stay visibly responsive
  digitalWrite(LED_INDICATOR, !digitalRead(LED_INDICATOR));

  // Serial Plotter check: one sample per line shows the waveform
  for (int i = 0; i < n; i++) {
    Serial.println(testSamples[i]);
  }

  // Rate check: should print ~16000 samples/sec
  samplesThisSecond += n;
  if (millis() - lastRateCheck >= 1000) {
    // (comment out the plotter loop above when reading this number)
    // Serial.print("samples/sec: "); Serial.println(samplesThisSecond);
    samplesThisSecond = 0;
    lastRateCheck = millis();
  }
#endif

  // Compute features

  // Run inference

  // Output a decision and drive leds

  // Advance the window
}
