/* Copyright 2019 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

LED Display Code from:
https://playground.arduino.cc/Main/LedControl/

example RTC code modified from : 
https://github.com/Makuna/Rtc/blob/master/examples/DS1307_Simple/DS1307_Simple.ino
*/

#include "LedControl.h"
#include <Wire.h>
#include <RtcDS1307.h>
#include <Servo.h> 
//#include <RtcDateTime.h>


// Pin 13 has an LED connected on most Arduino boards.  The clock board has one too.
#define PIN_BLINKY_LED  13
#define PIN_10_MIN_LED  2
#define PIN_20_MIN_LED  3
#define PIN_30_MIN_LED  4
#define PIN_40_MIN_LED  15
#define PIN_50_MIN_LED  16
#define PIN_60_MIN_LED  6
#define PIN_BUZZER      14
#define PIN_SERVO_MOTOR 5

// We also have pin 8 for the seperate colon on the 7-segment clock display

/*
 * Create a new LedControl object.
 * We use pins 12,11 and 10 on the Arduino for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
 * There will only be a single MAX7221 attached to the arduino
 */
#define PIN_LED_CONTROLLER_DATA 10
#define PIN_LED_CONTROLLER_CLK 9
#define PIN_LED_CONTROLLER_LOAD 8

#define LOOP_PERIOD     200


LedControl display = LedControl(PIN_LED_CONTROLLER_DATA, PIN_LED_CONTROLLER_CLK, PIN_LED_CONTROLLER_LOAD, 1);

// RTC setup
RtcDS1307<TwoWire> Rtc(Wire);

Servo servo_test; 
int angle = 0;    
int potentio = A0;    

const char* DayOfWeekString(int DoW) {
	switch (DoW) {
	case 0:
		return "Sun";
	case 1:
		return "Mon";
	case 2:
		return "Tue";
	case 3:
		return "Wed";
	case 4:
		return "Thu";
	case 5:
		return "Fri";
	case 6:
		return "Sat";
	default:
		return "---";
	}
}


void timeDisplay(LedControl* lc, int v, boolean secondDP) {
	int ones;
	int tens;
	int hundreds;
	int thousands;

	if(v > 9099)
		return;

	ones = v % 10;

	v = v/10;
	tens = v % 10;

	v = v/10;
	hundreds = v % 10;

	v = v/10;
	thousands = v;

	//Now print the number digit by digit
	if (thousands > 0 ) {
		lc->setDigit(0, 0,(byte)thousands,false);
	} else {
        lc->setChar(0, 0, ' ', false);
	}
	lc->setDigit(0,1,(byte)hundreds,secondDP);
	lc->setDigit(0,2,(byte)tens,false);
	lc->setDigit(0,3,(byte)ones,false);
}


void timePrint (RtcDateTime *time) {
    uint8_t hour = time->Hour();
    // handle 12-hour time silliness
    boolean pm = false;
    if (hour == 0) {
        hour = 12;
    } else if (hour > 12) {
        hour -= 12;
        pm = true;
    }
    uint8_t minute = time->Minute();
    uint8_t second = time->Second();


	// Give time at next five seconds
	Serial.print(DayOfWeekString(time->DayOfWeek()));
	Serial.print(" ");
	Serial.print(time->Year(), DEC);
	Serial.print("-");
	Serial.print(time->Month(), DEC);
	Serial.print("-");
	Serial.print(time->Day(), DEC);
	Serial.print(" ");
	Serial.print(hour, DEC);
	Serial.print(":");
    if (minute < 10) { Serial.print("0"); }
	Serial.print(minute, DEC);
	Serial.print(":");
    if (second < 10) { Serial.print("0"); }
	Serial.print(second, DEC);
    if (pm) {
        Serial.println(" pm");
    } else {
        Serial.println(" am");
    }
}


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

  pinMode(PIN_BUZZER, OUTPUT);
    Serial.begin(115200);
	pinMode(PIN_BLINKY_LED, OUTPUT);
//	pinMode(PIN_COLON_LED, OUTPUT);

	//wake up the MAX72XX from power-saving mode
	display.shutdown(0, false);
	//set a medium brightness for the Leds
	display.setIntensity(0,6);

    // setup setial port
    Serial.begin(115200);

	// Setup and check the RTC clock
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);
       
    // Start the I2C interface
    Wire.begin();

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) {
        if (Rtc.LastError() != 0) {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        } else {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");
            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    Serial.print("Compile time=");
	Serial.print(uint32_t(compiled), DEC);
    Serial.print(" :");
    timePrint(&compiled);

    RtcDateTime now = Rtc.GetDateTime();
    Serial.print("    RTC time=");
	Serial.print(uint32_t(now), DEC);
    Serial.print(" :");
    timePrint(&now);

    //Rtc.SetDateTime(compiled);
    if (uint32_t(now) < uint32_t(compiled)) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (uint32_t(now) > uint32_t(compiled)) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (uint32_t(now) == uint32_t(compiled)) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // disable the square wave output
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low); 

}

uint8_t count = 0;


// the loop routine runs over and over again forever:
void loop() {
    RtcDateTime now = Rtc.GetDateTime();
    timePrint(&now);
    uint8_t hour = now.Hour();
    uint8_t minute = now.Minute();

    // Display the 7-segment number
	int number = (hour * 100) + minute;
    timeDisplay(&display, number, false);

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

    tone(PIN_BUZZER, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(PIN_BUZZER);     // Stop sound...
    delay(1000);        // ...for 1sec

        count++;

    
    // blink the LED or colon
//	digitalWrite(PIN_COLON_LED, LOW);
//	delay(200);
//	digitalWrite(PIN_COLON_LED, HIGH);
//	delay(800);
}
