/*
  Requirement 3
  
  Wire a push-button between digital pin D2 and 3V3, with a 10 kΩ pull-down resistor from
  D2 to GND. Wire an external LED on D3 with a 220 Ω series resistor to GND. Write a sketch
  that reads D2 every loop iteration (polling), toggles the D3 LED on each fresh (debounced)
  button press, and logs each press to Serial with a timestamp


*/
void req3() {  
  // Get current state
  unsigned long current_time = millis();
  bool current_button_state = digitalRead(BUTTON_PIN); 

  // When the button is pressed, check if the button is debounced
  if (current_button_state) {
    if ((current_time - lastDebounceTime) > DEBOUNCE_DELAY) {
      // Toggle LED state
      ledState = !ledState;

      // Reset debounce timer
      lastDebounceTime = current_time;

      // And print a to serial monitor
      Serial.println("Button pressed @ " + String(current_time) + "ms | LED is " + (ledState ? "ON" : "OFF"));
    }
  }

  // Update LED state
  digitalWrite(LED_PIN, ledState);
  delay(100);
}