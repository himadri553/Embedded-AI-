# EECE.5862 Project Rubric
## Final Project Rubric — EECE.5862 (Embedded AI)
#### The course project is graduate-section only (EECE.5862). It is open-ended but must use the lab development kit (Arduino Nano 33 BLE Sense Rev2 minimum; optional Pi Pico + camera for image-based projects).

Note on weighting. The project is worth 6 points out of 100 in your overall course grade. The 100-point breakdown below is used internally to grade the project — your project score (out of 100) is then scaled to the 6-point contribution to your course total.

## Points Allocation
### Criterion	Points	What earns full credit
1	Problem definition & motivation	10	Clear statement of the task (inputs, outputs, success criteria). Real-world or personal motivation articulated. Scope is realistic for the lab kit.
2	Data	10	Dataset is appropriate, sufficient in size, and either properly collected or properly cited. Train/validation/test split is sensible. Data preprocessing is described.
3	Model design	10	Architecture choice is justified for the constraints (memory, MCU compute). Training procedure documented (loss, optimizer, epochs). Hyperparameter choices explained.
4	Embedded deployment	20	Model actually runs on the lab-kit hardware (Nano 33 BLE Sense Rev2, or Pico + camera if applicable). Conversion path (e.g., TFLite Micro, quantization) is shown. The on-device pipeline reads real sensor input — not a stored test vector.
5	Evaluation	15	Reports accuracy/precision/recall (or task-appropriate metrics) and on-device measurements: model size (KB), inference latency (ms), RAM usage. Compares against a baseline (random / lab-assignment model / simpler approach).
6	Demo	10	Live or recorded demonstration of the system working end-to-end on hardware. Failure modes shown honestly.
7	Written report	10	4–6 pages. Sections: problem, data, model, deployment, results, discussion, limitations. Figures and tables are legible and labeled. References included.
8	Deeper study	5	One of: (a) a quantitative ablation study isolating the effect of a design choice; (b) a comparison of two model architectures deployed on-device with side-by-side metrics; or (c) profiling-driven optimization showing before/after measurements (size, latency, or RAM).
9	Week 5 Checkpoint	10	Submitted by end of Week 5. Includes: (a) refined problem statement, (b) dataset acquired or collection plan in motion, (c) initial model trained off-device (any accuracy), (d) one open question or blocker stated. Short — 1 page or a 5-min video.
Notes
Incomplete deployment. If the model trains but does not run on the device, criterion 4 caps at 5/20 and criterion 6 caps at 3/10.
Missed checkpoint. If the Week 5 Checkpoint is not submitted, criterion 9 is 0 and no late credit is offered — its purpose is mid-project feedback, which late submission defeats.