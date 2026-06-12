/*
  Himadri Saha

  project_src.ino
    - Is the main entry of this program
    - Main loop does

*/
#include "helper.h"

void setup() {
  // Start terminal 
  Serial.begin(9600);
  
  // Pin Inits
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_INDICATOR, OUTPUT);

}

void loop() {

}
