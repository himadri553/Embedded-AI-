/*
  Himadri Saha

  config.h
    - contains all shared variables and function defs used across submodules in the software (pin defs, includes, etc.)

*/
#ifndef CONFIG_H
#define CONFIG_H

/* Pin Defs */
#define LED_RED A0
#define LED_GREEN A1
#define LED_BLUE A2
#define LED_INDICATOR A3

/* Audio Config */
// Shared with the off-device training pipeline - keep these in sync.
#define SAMPLE_RATE   16000   // Hz, standard for keyword spotting
#define PDM_CHANNELS  1       // mono
#define AUDIO_CHUNK   256     // int16 samples delivered per PDM callback

/* Serial Config */
#define SERIAL_BAUD   115200  // fast enough for Serial Plotter throughput

/* Audio Capture Layer interface (defined in audio.ino) */
#include <stdint.h>
bool audioBegin();
bool audioSamplesReady();
int  audioReadSamples(int16_t* dest, int maxSamples);

#endif  // CONFIG_H