// WICHTIG:
// KEINE SWITCH-CASE-STATEMENTS VERWENDEN!!1!111!


//includes
#include "ssd1306.h"
#include <EEPROM.h>
//#include <WiFiNINA.h>

//enums
enum frequency_type { AC,
                      DC,
                      DAC };
enum measurement_type { U,
                        I };

//pins
#define U_PIN A0
#define I_PIN A3
#define SWITCH_PIN 2

//locks
bool switchLock = false;

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20

#define U_DIVIDER 40.6f
#define I_DIVIDER 40.0f
#define I_OFFSET 508

#define SQRT2 1.4142

//varibales
enum frequency_type f_type = DC;
enum measurement_type m_type = I;
float current_U = 0.0f;
float current_I = 0.0f;

//EEPROM

//timer
#define MEASUREMENT_INTERVAL 1000
unsigned long m_timer = 0;

bool displayUpdate = true;

void setup() {

  Serial.begin(9600);

  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();

  ssd1306_clearScreen();

  Serial.println("Bereit");
}

void loop() {

  timer();
}

void timer() {

  unsigned long now = millis();

  if(!digitalRead(SWITCH_PIN) && !switchLock) {
    switchLock = true;
    if (m_type == U) {
      m_type = I;
      current_U = 0;
    } else {
      m_type = U;
      current_I = 0;
    }
  } else if (digitalRead(SWITCH_PIN)) {
    switchLock = false;
  }

  if (now - m_timer > MEASUREMENT_INTERVAL) {
    displayUpdate = true;
    m_timer = now;
    if (m_type == U) {
      current_U = measurement(U_PIN);
    } else {
      current_I = measurement(I_PIN);
    }
  }

  if (displayUpdate) updateDisplay();
}

void getData() {



}
