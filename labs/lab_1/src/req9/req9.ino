/*
  Req 9

  Option C.1 — Polling vs. interrupt latency characterization. Compare your polling (3)
  and interrupt (4) sketches quantitatively. Trigger 50 button presses at three different main-
  loop workloads (insert delay(1), delay(10), and delay(50) in the main loop). For each
  workload, report the worst-case response time from press to LED toggle. Write two short
  paragraphs analyzing the relationship between main-loop workload and worst-case response
  time in each style.
*/

#define USE_INTERRUPT 1
#define WORKLOAD_DELAY_MS 1

// Pin defs 
#define BUTTON_PIN 2
#define LED_PIN 3

// Config 
#define DEBOUNCE_DELAY 100

// Shared state (from lab1.h)
static volatile bool ledState = false;
static unsigned long lastDebounceTime = 0;
static volatile bool buttonPressed = false;

static volatile unsigned long press_timestamp_us = 0;
static unsigned long response_time_us = 0;

// Worst-case tracking across a batch of presses 
constexpr unsigned long kPressBatchSize = 50;
static unsigned long press_count = 0;
static unsigned long worst_case_response_us = 0;

// Records one press's response time into the rolling batch. Once
// kPressBatchSize presses have been seen, prints a "[RESP]" summary line
void recordPressResponse(unsigned long elapsed_us) {
  response_time_us = elapsed_us;
  press_count++;
  if (elapsed_us > worst_case_response_us) worst_case_response_us = elapsed_us;

  if (press_count >= kPressBatchSize) {
#if USE_INTERRUPT
    const char* style_name = "interrupt";
#else
    const char* style_name = "polling";
#endif
    Serial.println("[RESP] worst-case = " + String(worst_case_response_us) + " us over "
                   + String(press_count) + " presses (style=" + String(style_name)
                   + ", workload=" + String(WORKLOAD_DELAY_MS) + "ms)");

    press_count = 0;
    worst_case_response_us = 0;
  }
}

#if USE_INTERRUPT
// --- INTERRUPT from req4's buttonISR() ---
void buttonISR() {
  unsigned long current_time = micros();
  if ((current_time - lastDebounceTime) > (DEBOUNCE_DELAY * 1000)) {
    ledState = !ledState;
    lastDebounceTime = current_time;
    press_timestamp_us = current_time;  
    buttonPressed = true;  
  }
}
#endif

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

#if USE_INTERRUPT
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
  Serial.println("req9: interrupt style ready");
#else
  Serial.println("req9: polling style ready");
#endif
}

void loop() {
#if USE_INTERRUPT
  // --- Interrupt from req4() ---
  if (buttonPressed) {
    // Toggle pin now 
    digitalWrite(LED_PIN, ledState);
    recordPressResponse(micros() - press_timestamp_us);

    Serial.println("Button pressed (interrupt style) | response = " + String(response_time_us) + " us");
    buttonPressed = false;  // Clear flag
  }

#else
  // --- Polling from req3() ---
  unsigned long current_time = millis();
  bool current_button_state = digitalRead(BUTTON_PIN);

  if (current_button_state) {
    if ((current_time - lastDebounceTime) > DEBOUNCE_DELAY) {
      press_timestamp_us = micros();  // Mark press-detection time for latency measurement

      ledState = !ledState;
      lastDebounceTime = current_time;

      // Toggle the pin now — this is the "LED toggle" half of press-to-toggle.
      digitalWrite(LED_PIN, ledState);
      recordPressResponse(micros() - press_timestamp_us);

      Serial.println("Button pressed @ " + String(current_time) + "ms | LED is " + (ledState ? "ON" : "OFF") + " | response = " + String(response_time_us) + " us");
    }
  }
#endif

  delay(WORKLOAD_DELAY_MS);
}
