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
#include <embAI-project_inferencing.h>

/* Pin Defs */
#define LED_RED       A0
#define LED_GREEN     A1
#define LED_BLUE      A2
#define LED_INDICATOR A3
#define SERIAL_BAUD   115200

/* Audio Config */
#define PDM_CHANNELS                          1
#define PDM_GAIN                              127
#define SAMPLE_RATE                           EI_CLASSIFIER_FREQUENCY
#define EIDSP_QUANTIZE_FILTERBANK             0
#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 4

/* inference_t struct */
typedef struct {
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int  buf_count;
    unsigned int  n_samples;
} inference_t;

/* Audio function declarations */
bool audio_init();
bool audio_record();
int  audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
void audio_end();
void pdm_data_ready_inference_callback();
signed short* audio_get_buffer();
int audio_get_slice_size();

/* Inference function declarations */
void inference_init();
bool inference_run();

#endif  // CONFIG_H