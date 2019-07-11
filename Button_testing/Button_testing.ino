// Pin 13 has an LED connected on most Arduino boards.
#define PIN_BLINKY_LED      13
#define PIN_BUTTON          17

#define LOOP_PERIOD         10
#define PRINT_PERIOD        1000

// the setup routine runs once when you press reset:
void setup() {
	// initialize the digital pin as an output.
	pinMode(PIN_BLINKY_LED, OUTPUT);
    pinMode(PIN_BUTTON, INPUT);

     // setup setial port
    Serial.begin(115200);
}

boolean button_old = false;
uint16_t button_count = 0;
unsigned long print_time = 0;

// the loop routine runs over and over again forever:
void loop() {
    boolean button = digitalRead(PIN_BUTTON);
    if (button && !button_old) {
        button_count++;
    }
    button_old = button;

    if (millis() > (print_time + PRINT_PERIOD)) {
        Serial.print("button_count = ");
        Serial.print(button_count, DEC);
        Serial.println();
        print_time += PRINT_PERIOD;
    }
    delay(LOOP_PERIOD);
}
