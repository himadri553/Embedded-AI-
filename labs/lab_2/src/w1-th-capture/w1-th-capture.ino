// Lab 2 · Track W · A.1 — Temperature/Humidity capture   (deliverable: w1-th-capture.ino)
//
// Board: Arduino Nano 33 BLE Sense Rev2 (HS3003 sensor via Arduino_HS300x).
// Install Arduino_HS300x via Library Manager if you haven't (Lab 1 §2).
//
// Goal: once per second, read temperature (degC) and relative humidity (%)
// and print ONE CSV line over Serial so you can capture a dataset.
// Suggested columns:  millis,temp_c,humidity_pct
//
// Skeleton compiles/uploads as-is once the include resolves. Fill in
// each TODO(student). (Handout §A.1: >= 10 streamed lines is a deliverable.)

#include <Arduino_HS300x.h>

const unsigned long SAMPLE_INTERVAL_MS = 2000;  // 0.5 Hz sampling
unsigned long lastSample = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }  // wait for the Serial Monitor (USB native)

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize HS300x sensor!");
    while (1) { ; }  // halt — check the library install / board selection
  }

  // TODO(student): print a one-line CSV header so your capture file is
  // self-describing, e.g.  Serial.println("millis,temp_c,humidity_pct");
  Serial.println("millis,temp_c,humidity_pct");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSample < SAMPLE_INTERVAL_MS) return;
  lastSample = now;

  // TODO(student): read the sensor.
  //   float t = HS300x.readTemperature();   // degrees C
  //   float h = HS300x.readHumidity();       // % relative humidity
  float t = HS300x.readTemperature();
  float h = HS300x.readHumidity();

  // TODO(student): print ONE CSV line: now, t, h  (comma-separated,
  // Serial.println at the end). This is the stream you will later save
  // to a file and feed into training (B.1), and the window you describe
  // in A.2.
  Serial.print(now);
  Serial.print(",");
  Serial.print(t, 2);
  Serial.print(",");
  Serial.println(h, 2);
}