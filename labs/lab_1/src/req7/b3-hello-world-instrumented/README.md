# Lab 1 · B.3 starter — hello_world, ready to instrument

This folder is a **copy of `starter/hello_world/`** (the working TFLite
Micro sine-wave sketch from B.1/B.2), renamed to
`b3-hello-world-instrumented.ino` so it stays separate from your
pristine B.1/B.2 copy.

**Your task (handout §B.3):** measure inference latency. Wrap each
`Invoke()` call with `micros()`, and over **at least 100 successive
inferences** report the **min, mean, and max**. Also record the
**flash** and **RAM** usage the Arduino IDE prints after a successful
compile.

The two `TODO(student) B.3:` blocks in
`b3-hello-world-instrumented.ino` mark exactly where to add code:

1. In the `namespace { ... }` globals — declare your rolling-window
   accumulators (count, sum, min, max).
2. Around the `Invoke()` call in `loop()` — time it with `micros()`,
   accumulate, and every >= 100 inferences print a `[LAT]` summary
   line (then reset the window). **Use `Serial.print()` for the
   `[LAT]` line, not `MicroPrintf`** — the bundled TFLM v2.4.0-ALPHA
   `MicroPrintf` doesn't support the `l` length modifier, so `%lu`
   for the `unsigned long` that `micros()` returns prints garbage.
   Also note: `micros()` on the nRF52840 has ~4 µs resolution, so the
   min may collapse to one value across many samples — the mean over
   the window is the meaningful number.

**Build and flash it first, unmodified**, to confirm it runs (LED
fades, Serial Plotter shows the sine wave). Then add your
instrumentation. Submit this sketch as `b3-hello-world-instrumented/`
per the handout's submission list.

License/source for the underlying sketch: see `NOTICE.md` in this
folder (Apache-2.0, TensorFlow Authors / TinyML Cookbook 2E mirror).
