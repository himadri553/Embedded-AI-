/*
  Himadri Saha

  audio_capture.ino
    - Handles PDM microphone initialization and audio buffer management
    - Uses EI double-buffer pattern for continuous inference
*/

#include "config.h"

/* Private variables ------------------------------------------------------- */
static inference_t inference;
static bool record_ready = false;
static signed short *sampleBuffer;

/* Public functions -------------------------------------------------------- */

/*
  audio_init()
    - Allocates double buffers
    - Registers PDM ISR callback
    - Starts PDM at 16kHz mono
    - Returns false if allocation or PDM start fails
*/
bool audio_init() {
    inference.buffers[0] = (signed short *)malloc(EI_CLASSIFIER_SLICE_SIZE * sizeof(signed short));
    if (inference.buffers[0] == NULL) return false;

    inference.buffers[1] = (signed short *)malloc(EI_CLASSIFIER_SLICE_SIZE * sizeof(signed short));
    if (inference.buffers[1] == NULL) {
        free(inference.buffers[0]);
        return false;
    }

    sampleBuffer = (signed short *)malloc((EI_CLASSIFIER_SLICE_SIZE >> 1) * sizeof(signed short));
    if (sampleBuffer == NULL) {
        free(inference.buffers[0]);
        free(inference.buffers[1]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count  = 0;
    inference.n_samples  = EI_CLASSIFIER_SLICE_SIZE;
    inference.buf_ready  = 0;

    PDM.onReceive(&pdm_data_ready_inference_callback);
    PDM.setBufferSize((EI_CLASSIFIER_SLICE_SIZE >> 1) * sizeof(int16_t));

    if (!PDM.begin(PDM_CHANNELS, SAMPLE_RATE)) {
        ei_printf("ERR: Failed to start PDM!\n");
        return false;
    }

    PDM.setGain(PDM_GAIN);
    record_ready = true;
    return true;
}

/*
  audio_record()
    - Blocks until a fresh slice is ready in the inactive buffer
    - Returns false on buffer overrun
*/
bool audio_record() {
    bool ret = true;

    if (inference.buf_ready == 1) {
        ei_printf("ERR: Buffer overrun - decrease EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW\n");
        ret = false;
    }

    while (inference.buf_ready == 0) { delay(1); }
    inference.buf_ready = 0;

    return ret;
}

/*
  audio_signal_get_data()
    - EI signal callback: converts int16 samples to float for the classifier
*/
int audio_signal_get_data(size_t offset, size_t length, float *out_ptr) {
    numpy::int16_to_float(
        &inference.buffers[inference.buf_select ^ 1][offset],
        out_ptr,
        length
    );
    return 0;
}

/*
  audio_end()
    - Stops PDM and frees all buffers
*/
void audio_end() {
    PDM.end();
    free(inference.buffers[0]);
    free(inference.buffers[1]);
    free(sampleBuffer);
}

signed short* audio_get_buffer() {
    return inference.buffers[inference.buf_select ^ 1];
}

int audio_get_slice_size() {
    return EI_CLASSIFIER_SLICE_SIZE;
}

/* Private functions ------------------------------------------------------- */

/*
  pdm_data_ready_inference_callback()
    - PDM ISR: fills the active buffer with incoming samples
    - Swaps buffers and sets ready flag when slice is full
*/
void pdm_data_ready_inference_callback() {
    int bytesAvailable = PDM.available();
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (record_ready == true) {
        for (int i = 0; i < bytesRead >> 1; i++) {
            inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples) {
                inference.buf_select ^= 1;
                inference.buf_count  = 0;
                inference.buf_ready  = 1;
            }
        }
    }
}