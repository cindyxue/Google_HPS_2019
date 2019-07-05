// Pin 13 has an LED connected on most Arduino boards.
#define PIN_BLINKY_LED  13
#define PIN_10_MIN_LED  2
#define PIN_20_MIN_LED  3
#define PIN_30_MIN_LED  4
#define PIN_40_MIN_LED  15
#define PIN_50_MIN_LED  14
#define PIN_60_MIN_LED  6

#define LOOP_PERIOD     200


// the setup routine runs once when you press reset:
void setup() {
	// initialize the digital pin as an output.
	pinMode(PIN_BLINKY_LED, OUTPUT);
    Serial.begin(115200);
    
  pinMode(PIN_10_MIN_LED, OUTPUT);
    Serial.begin(115200);
  pinMode(PIN_20_MIN_LED, OUTPUT);
    Serial.begin(115200);
  pinMode(PIN_30_MIN_LED, OUTPUT);
    Serial.begin(115200);
  pinMode(PIN_40_MIN_LED, OUTPUT);
    Serial.begin(115200);
  pinMode(PIN_50_MIN_LED, OUTPUT);
    Serial.begin(115200);
  pinMode(PIN_60_MIN_LED, OUTPUT);
    Serial.begin(115200);
}


// the loop routine runs over and over again forever:
uint8_t count = 0;

void loop() { 
    digitalWrite(PIN_BLINKY_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_BLINKY_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);
    
    digitalWrite(PIN_10_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_10_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);

    digitalWrite(PIN_20_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_20_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);

    digitalWrite(PIN_30_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_30_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);

    digitalWrite(PIN_40_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_40_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);

    digitalWrite(PIN_50_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_50_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);

    digitalWrite(PIN_60_MIN_LED, HIGH);
    delay(LOOP_PERIOD);
    digitalWrite(PIN_60_MIN_LED, LOW);
    delay(LOOP_PERIOD);
    Serial.println(count, HEX);
    
    count++;
}
