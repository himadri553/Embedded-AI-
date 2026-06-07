/*
  Header file for lab 1
  - Change LAB_REQ to test each requirement of the lab
*/
#ifndef LAB1_H
#define LAB1_H

// Lab requirement selector
#define LAB_REQ 4

// Config
#define DEBOUNCE_DELAY 100

// Global static Vars
static volatile bool ledState = false;
static unsigned long lastDebounceTime = 0;
static bool req_setup = false;
static volatile bool buttonPressed = false;

// Pin defs
#define BUTTON_PIN 2
#define LED_PIN 3

// Functions for each requirement
void req1();
void req2();
void req3();
void req4();
void buttonISR();
void req5();

#endif