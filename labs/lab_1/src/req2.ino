/*
  Requirement 2
    - Blink with serial messages of millis in between state changes
*/

void req2() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON @ " + String(millis()) + " ms");
  delay(1000);                      
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF @ " + String(millis()) + " ms");   
  delay(1000);  
}