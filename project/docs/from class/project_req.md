Final Project Rubric — EECE.5862 (Embedded AI)
The course project is graduate-section only (EECE.5862). It is open-ended but must use the lab development kit (Arduino Nano 33 BLE Sense Rev2 minimum; optional Pi Pico + camera for image-based projects).

Note on weighting. The project is worth 6 points out of 100 in your overall course grade. The 100-point breakdown below is used internally to grade the project — your project score (out of 100) is then scaled to the 6-point contribution to your course total.

Final submission — what you must turn in
By the final deadline (Sun, Jul 12, 11:59 PM ET), upload one PDF to the Canvas project submission. The demo video and code are supplied as clickable links inside the PDF so you do not need to attach them separately — but you are welcome to also upload the video (mp4) and/or a code .zip alongside the PDF if you prefer.

The PDF must contain:

The report body. 4–6 pages, single-column, 11 pt or larger, with the following sections in this order — each labeled with a matching heading:
Problem & motivation — task definition, inputs, outputs, success criteria (feeds criterion 1)
Data — dataset origin, size, splits, preprocessing (feeds criterion 2)
Model design — architecture, training procedure, hyperparameters (feeds criterion 3)
Embedded deployment — conversion path, quantization, tensor-arena size, on-device I/O (feeds criterion 4)
Results — task metrics + on-device measurements (size in KB, latency in ms, RAM), compared to a baseline (feeds criterion 5)
Deeper study — the ablation / architecture comparison / profiling-driven optimization you chose (feeds criterion 8)
Discussion & limitations — what works, what does not, what you would do next
References — citations for datasets, papers, libraries, and any code you built on
A demo video link (in the Results section or on the first page): a recorded ≤ 4-minute video showing the system running end-to-end on the lab-kit hardware. Host on Google Drive / YouTube / OneDrive with sharing set so I can open it without a permission request, or upload the mp4 directly to Canvas alongside the PDF. Live demos are not accepted (see criterion 6).
A code / repository link (in the References section or on the first page). Any one of:
a public GitHub / GitLab URL, or
a private GitHub / GitLab repo with me added as a collaborator — GitHub username yanluo, or
a .zip of the repo uploaded to Canvas alongside the PDF.
Whichever option you pick, the repo/zip must contain the training script, the conversion script, the Arduino sketch, and a README.md with a one-paragraph reproduction recipe.
A submission missing any of items 1–3 is treated as incomplete: the corresponding rubric criteria receive 0 and no late credit is offered on the missing item. Broken or permission-locked links count as missing.

#	Criterion	Points	What earns full credit
1	Problem definition & motivation	10	Clear statement of the task (inputs, outputs, success criteria). Real-world or personal motivation articulated. Scope is realistic for the lab kit.
2	Data	10	Dataset is appropriate, sufficient in size, and either properly collected or properly cited. Train/validation/test split is sensible. Data preprocessing is described.
3	Model design	10	Architecture choice is justified for the constraints (memory, MCU compute). Training procedure documented (loss, optimizer, epochs). Hyperparameter choices explained.
4	Embedded deployment	20	Model actually runs on the lab-kit hardware (Nano 33 BLE Sense Rev2, or Pico + camera if applicable). Conversion path (e.g., TFLite Micro, quantization) is shown. The on-device pipeline reads real sensor input — not a stored test vector.
5	Evaluation	15	Reports accuracy/precision/recall (or task-appropriate metrics) and on-device measurements: model size (KB), inference latency (ms), RAM usage. Compares against a baseline (random / lab-assignment model / simpler approach).
6	Demo	10	Recorded demonstration (video) of the system working end-to-end on hardware. Failure modes shown honestly. Live demos are not accepted — a recording is required so I can review at the same detail across all submissions.
7	Written report	10	The 4–6 page PDF report described in the "Final submission" section above, with all eight required sections present and correctly labeled. Figures and tables are legible and labeled with captions. References are complete and formatted consistently.
8	Deeper study	5	One of: (a) a quantitative ablation study isolating the effect of a design choice; (b) a comparison of two model architectures deployed on-device with side-by-side metrics; or (c) profiling-driven optimization showing before/after measurements (size, latency, or RAM).
9	Week 5 Checkpoint	10	Submitted by end of Week 5. Includes: (a) refined problem statement, (b) dataset acquired or collection plan in motion, (c) initial model trained off-device (any accuracy), (d) one open question or blocker stated. Short — 1 page or a 5-min video.
Penalties
Incomplete deployment. If the model trains but does not run on the device, criterion 4 caps at 5/20 and criterion 6 caps at 3/10.
Missed checkpoint. If the Week 5 Checkpoint is not submitted, criterion 9 is 0 and no late credit is offered — its purpose is mid-project feedback, which late submission defeats.