/*
  Himadri Saha

  decision.ino
    - Reads EI classifier result
    - Applies argmax + confidence threshold
    - Resolves to one of {red, green, blue, noise, unknown}
*/

/* Public functions -------------------------------------------------------- */

/*
  decision_get_label()
    - Runs inference on current audio slice
    - Returns the winning label string if confidence >= CONFIDENCE_THRESHOLD
    - Returns "noise" if no class meets the threshold
*/
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);

const char* decision_run() {
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data     = &audio_signal_get_data;

    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, false);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: run_classifier_continuous() failed (%d)\n", r);
        return "noise";
    }

    // Only make a decision once a full model window is accumulated
    if (++print_results < EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW) {
        return "noise";
    }
    print_results = 0;

    // Argmax over all classes
    const char* top_label = "noise";
    float       top_score = 0.0f;

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > top_score) {
            top_score = result.classification[ix].value;
            top_label = result.classification[ix].label;
        }
    }

    // Threshold gate
    if (top_score < CONFIDENCE_THRESHOLD) {
        ei_printf("Below threshold (%.2f) — defaulting to noise\n", top_score);
        return "noise";
    }

    ei_printf("Decision: %s (%.2f)\n", top_label, top_score);
    return top_label;
}