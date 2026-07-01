# Embedded AI Final Project - Vocal Word Classification
### Himadri Saha
#### Repo Link: https://github.com/himadri553/Embedded-AI-

## Description
This project implements a keyword-spotting (KWS) system on the Arduino Nano 33 BLE Sense Rev2 that classifies spoken color words — "red," "green," and "blue" — in  real time using the onboard PDM microphone, and drives a corresponding LED to  indicate the recognized word. Audio is captured continuously via a double-buffered PDM interrupt pipeline, classified on-device using a TensorFlow Lite Micro model exported from Edge Impulse, and gated behind an 80% confidence threshold to reject background noise and out-of-vocabulary speech. The system runs entirely on-device with no external compute, achieving ~259ms end-to-end latency per inference.

## Reproduction Recipe
Open `project_src/project_src.ino` in Arduino IDE with the Arduino 
Mbed OS Nano Boards core (v4.6.0) installed, then add the exported Edge Impulse 
library from `model/ei-embai-project-arduino-1.0.2-impulse-#4.zip` via 
Sketch → Include Library → Add .ZIP Library. Wire three 5mm LEDs through 220Ω 
resistors to their configured GPIO pins (as defined in `config.h`) on the 
breadboard, select "Arduino Nano 33 BLE" as the board target, and upload. On 
boot, the onboard PDM microphone begins continuous inference — speaking "red," 
"green," or "blue" within range lights the matching LED, while silence or 
unrecognized speech defaults to no LED (classified as noise/unknown).

## Directory 
```
project/
├── README.md
├── docs/
│   ├── Project Midpoint Report.docx
│   ├── Project Final Report.docx
│   ├── TODO.md
│   ├── demo.mov
│   └── from class/
│       └── project_req.md
├── model/
│   └── ei-embai-project-arduino-1.0.2-impulse-#4.zip   # exported Edge Impulse Arduino library
└── project_src/
    ├── project_src.ino     
    ├── config.h            
    ├── audio_capture.ino   
    ├── inference.ino       
    ├── decision.ino        
    └── output.ino          
```

## Software Architecture
The software for this project is split up into "layers", with each layer containing code that serves a similar purpose. This makes it easier to make changes, and for new users to better understand the code. 
- Audio Capture Layer (audio_capture.ino): Initializes the onboard PDM microphone and continuously fills a double-buffer with raw 16-bit PCM samples at 16 kHz using an interrupt-driven callback, ensuring audio is always being captured without blocking the main loop.
- Inference Layer (inference.ino): Initializes the Edge Impulse continuous classifier and provides the TFLM interpreter with the model, tensor arena, and runtime needed to execute the forward pass.
- Decision Layer (decision.ino): Pulls the current audio slice from the audio buffer, runs run_classifier_continuous() to get softmax scores for all five classes, applies argmax to find the top class, and gates the result against an 80% confidence threshold — defaulting to "noise" if no class meets it.
- Output Layer (output.ino): Receives the resolved label string from the decision layer and drives the corresponding GPIO pin to blink the matching LED once, doing nothing for "noise" or "unknown".

How the layers connect:
- Audio Capture → Decision: audio_signal_get_data() is registered as the EI signal_t callback inside decision_run(), giving the classifier direct access to the inactive audio double-buffer.
- Inference → Decision: run_classifier_continuous() and run_classifier_init() are EI library calls owned by inference.ino (init) and decision.ino (runtime), with the result struct ei_impulse_result_t produced inside decision_run().
- Decision → Output: decision_run() returns a const char* label string which is passed directly into output_drive() in the main loop.
- config.h → All layers: all shared constants, pin definitions, the inference_t struct, and all cross-file function declarations live in config.h, which every .ino file includes.
- project_src.ino → All layers: setup() calls output_init(), audio_init(), and inference_init() in order; loop() calls audio_record(), decision_run(), and output_drive() sequentially on every iteration.

## Stats 
### Model Estimates
Latency: 259ms 
RAM: 19.8K
Flash: 32.6K

### Inference Stats
(DSP: 51ms, Classification: 19ms)