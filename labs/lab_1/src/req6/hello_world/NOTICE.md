# `hello_world` — source and license

This directory is a **verbatim mirror** of the TensorFlow Lite for
Microcontrollers `hello_world` example, taken from the upstream Google
repository so students don't have to chase it down separately.

## Provenance

- Upstream: <https://github.com/tensorflow/tflite-micro-arduino-examples/tree/main/examples/hello_world>
- License: **Apache-2.0** (see LICENSE in upstream repo; copyright
  header in every file)
- Upstream status: archived 2023-12 (no longer maintained — but stable
  for our use; the sketch demonstrates a model that hasn't changed)
- Mirrored on: 2026-05-13

## What it does

Trains-in-advance and bundles a tiny model that learns the sine
function: given an angle x in radians, predict sin(x). The sketch
sweeps x from 0 to 2π, runs inference on the Nano's Cortex-M4F, and
emits the predicted value over the Serial Monitor. On boards with an
RGB LED it also fades the LED brightness in proportion to the
prediction. It's the canonical "smallest meaningful TFLM inference"
demo — small enough to fit in tens of KB, complete enough to exercise
the whole interpreter stack.

## How to use in Lab 1

1. Make sure `Arduino_TensorFlowLite` is installed (see
   `../../Arduino_TensorFlowLite.zip.NOTICE.md`).
2. In Arduino IDE 2.x: **File → Open…** and pick `hello_world.ino` from
   this directory. The IDE will open the entire sketch folder.
3. **Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE**,
   then **Tools → Port → <your Nano's port>**.
4. Click **Verify** (✓) to compile. The first build takes a couple of
   minutes — TFLite Micro pulls in a lot of headers.
5. Click **Upload** (→). After flashing, open the Serial Monitor at
   9600 baud and watch the predictions stream past.
