/*
  Himadri Saha

  src.ino
    - Acts as the main entry for this lab
    - Change LAB_REQ to test each requremnet of the lab in /lab1.h
  
  TODO:
    - Req 1: X
    - Req 2: X
    - Req 3: 
*/
#include "lab1.h"

void setup() {
  // Serial Init
  Serial.begin(9600);
  
  // Pin Inits
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  #if LAB_REQ == 1
    req1();
  #elif LAB_REQ == 2
    req2();
  #elif LAB_REQ == 3
    req3();
  #endif
}
