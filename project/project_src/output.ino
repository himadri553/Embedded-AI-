/*
  Himadri Saha

  output.ino
    - Receives decision label from decision.ino
    - Drives corresponding LED via GPIO
*/

/* Public functions -------------------------------------------------------- */

/*
  output_init()
    - Initializes LED pins as outputs
    - Ensures all LEDs start OFF
    - Call once in setup()
*/
void output_init() {
    pinMode(LED_RED,       OUTPUT);
    pinMode(LED_GREEN,     OUTPUT);
    pinMode(LED_BLUE,      OUTPUT);
    pinMode(LED_INDICATOR, OUTPUT);

    digitalWrite(LED_RED,       LOW);
    digitalWrite(LED_GREEN,     LOW);
    digitalWrite(LED_BLUE,      LOW);
    digitalWrite(LED_INDICATOR, LOW);
}

/*
  output_drive()
    - Takes decision label string
    - Blinks matching LED once
    - Does nothing for "noise" or "unknown"
*/
void output_drive(const char* label) {
    if (strcmp(label, "Red") == 0)        { blink(LED_RED);   }
    else if (strcmp(label, "Green") == 0) { blink(LED_GREEN); }
    else if (strcmp(label, "Blue") == 0)  { blink(LED_BLUE);  }
    // noise and unknown → do nothing
}

/* Private functions ------------------------------------------------------- */

/*
  blink()
    - Blinks a single LED pin once
*/
void blink(int pin) {
    digitalWrite(pin, HIGH); delay(200);
    digitalWrite(pin, LOW);  delay(200);
}