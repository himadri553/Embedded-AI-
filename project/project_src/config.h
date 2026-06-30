/*
  Himadri Saha

  config.h
    - contains all shared variables and function defs used across submodules in the software (pin defs, includes, etc.)

*/
#ifndef CONFIG_H
#define CONFIG_H

/* All libs */
#include <PDM.h>
#include <stdint.h>

/* Pin Defs and other setup */
#define LED_RED       A0
#define LED_GREEN     A1
#define LED_BLUE      A2
#define LED_INDICATOR A3
#define SERIAL_BAUD   9600  

/* Audio Config */
#define SAMPLE_RATE   16000   
#define PDM_CHANNELS  1       
#define AUDIO_CHUNK   256     // int16 samples delivered per PDM callback
short sampleBuffer[256];
volatile int samplesRead = 0;
void onPDMdata();

#endif  // CONFIG_H