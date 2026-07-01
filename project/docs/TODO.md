[] Get all the stats for report study
Full list of stats needed
- [ ] Train/val/test split counts per class — NOT YET FOUND (pull from EI Data Acquisition page)
- [ ] DSP block parameters (frame length/stride, # coefficients) — NOT YET FOUND (pull from EI DSP block config page)
- [ ] Model layer count/parameter count — NOT YET FOUND (pull from EI model architecture page)
- [x] Final validation accuracy — FOUND: 91.18%
- [ ] Final training accuracy/loss — NOT YET FOUND
- [ ] Training time — NOT YET FOUND
- [ ] Exact tensor arena size (bytes) — NOT YET FOUND (check EI deployment page or Arduino serial output)
- [ ] Quantization status (INT8 or float32) — NOT YET FOUND (confirm from EI export settings)
- [x] Known confusion pattern — FOUND: green→red confusion (~25%)
- [ ] Full per-class precision/recall/F1 — NOT YET FOUND (pull from EI Model Testing page)
- [ ] Full confusion matrix (all classes) — NOT YET FOUND
- [x] Model size (flash) — FOUND: 32.6 KB
- [x] RAM usage — FOUND: 19.8 KB
- [x] Total inference latency — FOUND: 259 ms
- [x] DSP latency — FOUND: 51 ms
- [x] Classification latency — FOUND: 19 ms
- [ ] Latency reconciliation — NOT YET FOUND (51ms + 19ms = 70ms; the remaining ~189ms of the 259ms total is unaccounted for — needs explanation, likely anomaly detection/other EI stages, or audio buffer wait time)
- [ ] Baseline comparison numbers — NOT YET FOUND (need to pick and run a baseline: random-guess 20%, amplitude-threshold heuristic, or lab-assignment model)
- [ ] Deeper study before/after metrics (size, latency, RAM, accuracy) — NOT YET FOUND (depends on which deeper study option is chosen)

[x] Report Outline
[] Complete Report 
[x] Complete README