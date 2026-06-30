/*
  Himadri Saha

  project_src.ino
    - Is the main entry of this program
    - TESTING: audio_init() and audio_record() only
*/
#include "config.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) { ; }

  output_init();

  Serial.println("Initializing audio...");
  if (!audio_init()) {
    Serial.println("ERR: audio_init() failed — halting.");
    while (1) { ; }
  }
  Serial.println("audio_init() OK");

  inference_init();
  Serial.println("Ready.");
}

void loop() {
  if (!audio_record()) {
    Serial.println("ERR: audio_record() buffer overrun");
    return;
  }

  const char* label = decision_run();
  output_drive(label);
  Serial.println(label);
}