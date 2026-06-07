/*
    Requirement 4

    Rewrite (3) using an attached interrupt handler (attachInterrupt()) on the rising edge of
    D2. The ISR should set a volatile bool flag that the main loop consumes. Add one Serial
    line per detected press that reports the number of microseconds elapsed in loop() (use
    micros()) since the previous button even
*/
void buttonISR() {
  // Check debounce
  unsigned long current_time = micros();
  if ((current_time - lastDebounceTime) > (DEBOUNCE_DELAY * 1000) ) {
    ledState = !ledState;
    lastDebounceTime = current_time;
    buttonPressed = true;  // Set flag for main loop
  }
}

void req4() {
  // Attach Interupt
  if (!req_setup) {
    // Attach interrupt to BUTTON_PIN, trigger on rising edge
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
    req_setup = true;
    Serial.println("setup for req 4 is complete");
  }

  // Check if button was pressed and handle in main loop
  if (buttonPressed) {
    Serial.println("Button pressed: ");
    buttonPressed = false;  // Clear flag
  }

  // Update LED State
  digitalWrite(LED_PIN, ledState);
  delay(100);
}