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
#define BTN1_PIN 2
#define BTN2_PIN 4

//locks
bool btn1Lock = false;
bool btn2Lock = false;
bool doubleLock = false;

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20  //für 50Hz
#define MIN_FREQ 50
#define FREQ_ITR 10

#define U_DIVIDER 40.3f
#define I_DIVIDER 40.0f
uint16_t I_OFFSET = 512;

#define SQRT2 1.4142

//varibales
enum frequency_type f_type = DAC;
enum measurement_type m_type = I;
float current_U = 0.0f;
float current_I = 0.0f;
float freq = 0.0;

//EEPROM
#define EEPROM_CHECK_ADDR 0
#define EEPROM_CHECK_VALUE 1

//timer
#define MEASUREMENT_INTERVAL 1000
uint32_t m_timer = 0;

void setup() {

  Serial.begin(115200);

  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();

  getData();
  Serial.println("Kalibrierung...");
  delay(1000);
  I_OFFSET = ACSCal(I_PIN);

  Serial.println("Bereit");
  Serial.println("t\tmtype\tftype\tf in Hz\tU in V\tI in A");
  Serial.println("----------------------------------------------");
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
      getFreq(I_PIN);
      current_I = getCurrent(I_PIN);
    }
    updateDisplay();
    serialInfoTab();
    //serialInfoBlock();
  }
}

void controls() {

  if (!digitalRead(BTN1_PIN) && !digitalRead(BTN2_PIN) && !doubleLock) {
    doubleLock = true;
    return;
  } else if (!(!digitalRead(BTN1_PIN) && !digitalRead(BTN2_PIN)) && doubleLock) {
    doubleLock = false;
    btn1Lock = false;
    btn2Lock = false;
    if (f_type == DC) {
      f_type = DAC;
    } else if (f_type == DAC) {
      f_type = AC;
    } else {
      f_type = DC;
    }
    return;
  }

  if (!digitalRead(BTN1_PIN) && !btn1Lock) {
    btn1Lock = true;
  } else if (digitalRead(BTN1_PIN) && btn1Lock) {
    btn1Lock = false;
    if (m_type == U) {
      m_type = I;
      current_U = 0;
    } else {
      m_type = U;
      current_I = 0;
    }
  }

  if (!digitalRead(BTN2_PIN) && !btn2Lock) {
    btn2Lock = true;
  } else if (digitalRead(BTN2_PIN) && btn2Lock) {
    btn2Lock = false;
    I_OFFSET = ACSCal(I_PIN);
  }
}

void getData() {

  if (EEPROM.read(EEPROM_CHECK_ADDR) != EEPROM_CHECK_VALUE) {
    EEPROM.put(EEPROM_CHECK_ADDR, EEPROM_CHECK_VALUE);
  } else {
  }
}
