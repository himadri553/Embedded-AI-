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