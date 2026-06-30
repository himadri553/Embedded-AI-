## Project Ideation
Using the onboard microphone on the Arduino Nano, classify someone saying the words (red, blue, green), and having LEDs indicate the detected word. 

## File Structure
```
```

## TODO
- MVP code for getting microphone data
- 

## Software arch: brainstorm
- main: main loop
- drivers: separate functions for LEDs and microphone

## Software Layers: Brainstorm:
- Audio Capture Layer: PDM mic → DMA buffer → 16 kHz PCM samples, interrupt-driven to avoid blocking the main loop
- Preprocessing Layer: Sliding window over raw samples → MFCC feature extraction → normalized 2D array staged as model input tensor
- Inference Layer: TFLM interpreter reads tensor arena, runs forward pass on MFCC input, outputs softmax scores per class
- Decision Layer: Argmax over output scores + confidence threshold to filter false positives → resolves to one of {red, green, blue, silence}
- Output Layer: Decision triggers GPIO writes to corresponding LED; ISR-deferred to keep inference loop clean

## Workflow
1. Audio Capture — get PDM ISR + buffer working first. Verify with the k1-audio-capture.ino level-reporting pattern (print RMS/peak to Serial). Nothing downstream works without clean PCM.
2. Preprocessing — feed the captured PCM into the sliding window + MFCC pipeline. Verify offline: pipe known audio through it and check MFCC output shape/values against a Python reference before touching TFLM.
3. Inference — load your exported EI model, wire the MFCC output to the input tensor, call Invoke(). Verify with a static test vector first (hardcoded MFCC frame) before connecting live audio.
4. Decision — add argmax + confidence threshold on top of live inference output. Tune threshold against Serial-printed scores before LEDs are involved.
5. Output — wire the decision to GPIO. This is the easiest layer and requires everything above to be working, so it goes last.