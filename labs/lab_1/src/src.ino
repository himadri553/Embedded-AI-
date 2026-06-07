/*
  Himadri Saha

  src.ino
    - Acts as the main entry for this lab
    - Change LAB_REQ to test each requirement of the lab in ./lab1.h
  
  TODO:
    - Req 1: X
    - Req 2: X
    - Req 3: X
    - Req 4: 
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
  #elif LAB_REQ == 4
    req4();
  #elif LAB_REQ == 5
    req5();
  #endif
}
