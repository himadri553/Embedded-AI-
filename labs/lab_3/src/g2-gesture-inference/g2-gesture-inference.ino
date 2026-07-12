#include <Abumere17-project-1_inferencing.h>
#include <Arduino_BMI270_BMM150.h>
#include <string.h>

const int kLedRed = LEDR;
const int kLedGreen = LEDG;
const int kLedBlue = LEDB;

constexpr int kWindowN = 3;
constexpr int kVotesMin = 2;
constexpr float kConfMin = 0.70f;
constexpr unsigned long kReArmMs = 1000;
constexpr int kRestClassIx = 2;

int voteWindow[kWindowN] = {kRestClassIx, kRestClassIx, kRestClassIx};
int windowCursor = 0;
int prevActedIx = kRestClassIx;
unsigned long prevActMs = 0;

static float featBuf[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

int rawFeatureGetData(size_t offset, size_t length, float *outPtr) {
  memcpy(outPtr, featBuf + offset, length * sizeof(float));
  return 0;
}

int majorityClass() {
  for (int i = 0; i < kWindowN; i++) {
    int pick = voteWindow[i];
    int tally = 0;
    for (int j = 0; j < kWindowN; j++) {
      if (voteWindow[j] == pick) tally++;
    }
    if (tally >= kVotesMin) return pick;
  }
  return -1;
}

void allOff() {
  digitalWrite(kLedRed, HIGH);
  digitalWrite(kLedGreen, HIGH);
  digitalWrite(kLedBlue, HIGH);
}

void blinkColor(int pin) {
  allOff();
  digitalWrite(pin, LOW);
  delay(200);
  digitalWrite(pin, HIGH);
  delay(200);
}

void actOnStableDetection(int classIx, const char *label) {
  unsigned long stamp = millis();
  if (classIx == prevActedIx && (stamp - prevActMs) < kReArmMs) return;

  if (strcasecmp(label, "punch") == 0) blinkColor(kLedRed);
  else if (strcasecmp(label, "flex") == 0) blinkColor(kLedGreen);
  else if (strcasecmp(label, "circle") == 0) blinkColor(kLedBlue);

  Serial.print("ACT ");
  Serial.print(label);
  Serial.print(" @ ");
  Serial.print(stamp);
  Serial.println(" ms");

  prevActedIx = classIx;
  prevActMs = stamp;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { }

  pinMode(kLedRed, OUTPUT);
  pinMode(kLedGreen, OUTPUT);
  pinMode(kLedBlue, OUTPUT);
  allOff();

  if (!IMU.begin()) {
    Serial.println("IMU.begin() failed");
    while (1) { }
  }

  run_classifier_init();

  Serial.println("Gesture inference ready");
  Serial.print("Labels: ");
  Serial.println(EI_CLASSIFIER_LABEL_COUNT);
  Serial.print("Frame size: ");
  Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  Serial.print("Frequency: ");
  Serial.println(EI_CLASSIFIER_FREQUENCY);
}

void loop() {
  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 6) {
    while (!IMU.accelerationAvailable() || !IMU.gyroscopeAvailable()) { }

    IMU.readAcceleration(featBuf[ix], featBuf[ix + 1], featBuf[ix + 2]);
    IMU.readGyroscope(featBuf[ix + 3], featBuf[ix + 4], featBuf[ix + 5]);
    delay(1000 / EI_CLASSIFIER_FREQUENCY);
  }

  signal_t signal;
  signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  signal.get_data = &rawFeatureGetData;

  ei_impulse_result_t result = {};
  EI_IMPULSE_ERROR status = run_classifier(&signal, &result, false);
  if (status != EI_IMPULSE_OK) {
    Serial.print("Classifier failed: ");
    Serial.println((int)status);
    return;
  }

  int rawTopIx = 0;
  float bestConf = result.classification[0].value;
  for (size_t i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (result.classification[i].value > bestConf) {
      bestConf = result.classification[i].value;
      rawTopIx = (int)i;
    }
  }

  int filteredTopIx = rawTopIx;
  if (bestConf < kConfMin) filteredTopIx = kRestClassIx;

  voteWindow[windowCursor] = filteredTopIx;
  windowCursor = (windowCursor + 1) % kWindowN;
  int stableIx = majorityClass();

  Serial.print("top=");
  Serial.print(result.classification[rawTopIx].label);
  Serial.print(" conf=");
  Serial.print(bestConf, 2);
  Serial.print(" stable=");
  if (stableIx >= 0) Serial.print(ei_classifier_inferencing_categories[stableIx]);
  else Serial.print("none");
  Serial.print(" dsp_ms=");
  Serial.print(result.timing.dsp);
  Serial.print(" cls_ms=");
  Serial.println(result.timing.classification);

  if (stableIx >= 0 && stableIx != kRestClassIx) {
    actOnStableDetection(stableIx, ei_classifier_inferencing_categories[stableIx]);
  } else if (stableIx == kRestClassIx) {
    prevActedIx = kRestClassIx;
  }
}
