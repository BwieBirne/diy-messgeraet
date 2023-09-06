// WICHTIG:
// KEINE SWITCH-CASE-STATEMENTS VERWENDEN!


//includes
#include "ssd1306.h"  //Alexey Dynda
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
#define CAL_PIN 4

//locks
bool switchLock = false;
bool calLock = false;

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20  //für 50Hz
#define MIN_FREQ 50

#define U_DIVIDER 40.3f
#define I_DIVIDER 40.0f
uint16_t I_OFFSET = 512;

#define SQRT2 1.4142

//varibales
enum frequency_type f_type = DC;
enum measurement_type m_type = I;
float current_U = 0.0f;
float current_I = 0.0f;
float freq = 0.0;

//EEPROM
#define EEPROM_CHECK_ADDR 0
#define EEPROM_CHECK_VAL 1

//timer
#define MEASUREMENT_INTERVAL 1000
uint32_t m_timer = 0;

void setup() {

  Serial.begin(115200);

  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(CAL_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();

  getData();
  Serial.println("Kalibrierung...");
  delay(1000);
  I_OFFSET = ACSCal(I_PIN);

  Serial.println("Bereit");
  Serial.println("t\tm\tf in Hz\tU in V\tI in A");
  Serial.println("---------------------------------------");
}

void loop() {

  timer();
  controls();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > MEASUREMENT_INTERVAL) {
    m_timer = now;
    if (m_type == U) {
      current_U = getVoltage(U_PIN);
    } else {
      getftype(I_PIN);
      current_I = getCurrent(I_PIN);
    }
    updateDisplay();
    serialInfoTab();
    //serialInfoBlock();
  }
}

void controls() {

  if (!digitalRead(SWITCH_PIN) && !switchLock) {
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

 if (!digitalRead(CAL_PIN) && !switchLock) {
    calLock = true;
    I_OFFSET = ACSCal(I_PIN);
  } else if (digitalRead(CAL_PIN)) {
    calLock = false;
  }
}

void getData() {

  if (EEPROM.read(EEPROM_CHECK_ADDR) != EEPROM_CHECK_VAL) {
    EEPROM.put(EEPROM_CHECK_ADDR, EEPROM_CHECK_VAL);
  } else {
  }
}
