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

  Serial.println("Initializing audio...");
  if (!audio_init()) {
    Serial.println("ERR: audio_init() failed — halting.");
    while (1) { ; }
  }
  Serial.println("audio_init() OK");
  Serial.print("Slice size: ");
  Serial.println(EI_CLASSIFIER_SLICE_SIZE);
  Serial.print("Sample rate: ");
  Serial.println(SAMPLE_RATE);
}

void loop() {
  if (!audio_record()) {
    Serial.println("ERR: audio_record() buffer overrun");
    return;
  }

  // Print peak amplitude of the inactive buffer to confirm audio is flowing
  signed short *buf = audio_get_buffer();
  int16_t peak = 0;
  for (int i = 0; i < audio_get_slice_size(); i++) {
    int16_t v = abs(buf[i]);
    if (v > peak) peak = v;
  }

  Serial.print("Slice ready | peak: ");
  Serial.println(peak);
}