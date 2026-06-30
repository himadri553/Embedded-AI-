/*
  Himadri Saha

  inference.ino
    - Handles EI classifier initialization and continuous inference
    - Reads from audio double buffer via audio_signal_get_data()
    - Prints raw classification scores over Serial for all classes
*/

/* Private variables ------------------------------------------------------- */
static bool debug_nn = false;
static int  print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);

/* Public functions -------------------------------------------------------- */

/*
  inference_init()
    - Initializes the EI continuous classifier
    - Call once in setup()
*/
void inference_init() {
    run_classifier_init();
    ei_printf("Classifier initialized\n");
    ei_printf("  Classes : %d\n", EI_CLASSIFIER_LABEL_COUNT);
    ei_printf("  Slice size: %d\n", EI_CLASSIFIER_SLICE_SIZE);
    ei_printf("  Slices per window: %d\n", EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
}

/*
  inference_run()
    - Builds signal from active audio buffer
    - Runs one continuous inference slice
    - Prints scores when a full model window has been accumulated
    - Returns true on success, false on classifier error
*/
bool inference_run() {
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data     = &audio_signal_get_data;

    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: run_classifier_continuous() failed (%d)\n", r);
        return false;
    }

    // Only print once a full model window has been accumulated
    if (++print_results >= EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW) {
        ei_printf("Scores (DSP: %dms, Classification: %dms):\n",
            result.timing.dsp,
            result.timing.classification);

        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            ei_printf("  %s: %.5f\n",
                result.classification[ix].label,
                result.classification[ix].value);
        }

        print_results = 0;
    }

    return true;
}