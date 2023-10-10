// NOTIZEN
// WICHTIG: KEINE SWITCH-CASE-STATEMENTS VERWENDEN!

//includes
#include "ssd1306.h"  //Alexey Dynda
#include <EEPROM.h>
//#include <WiFiNINA.h>
//#include <ArduinoBLE.h>

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

//configuration
typedef struct configuration {
  const uint16_t MEASUREMENT_INTERVAL = 1000;  //milliseconds
  const uint8_t WAVE_ITR = 10;
  const uint8_t CAL_ITR = 500;
  const uint8_t MIN_FREQ = 50;
  const uint8_t FREQ_ITR = 10;
};

//calibration
typedef struct calibration {
  const float U_DIVIDER = 37.0f;             //Norm: 1V
  const float I_DIVIDER = 40.0f;             //Norm: 0.2A
  const float U_CORRECTION_FACTOR = 0.088f;  //Norm: 1V
  const float U_CORRECTION_NORM = 3.3f;      //Norm: 1V
  int8_t I_OFFSET = 0;
  const uint8_t FREQ_DC_BOUND = 16;
  const uint8_t FREQ_DAC_BOUND = 4;
};

//measurement
typedef struct measurement {
  uint32_t time = 0;  //milliseconds
  enum frequency_type f_type = DC;
  enum measurement_type m_type = U;
  float current_U = 0.0f;
  float current_I = 0.0f;
  float freq = 0.0f;
  uint16_t periodduration = 20000;  //microseconds
};

#define SQRT2 1.4142

struct configuration config;
struct calibration cal1;
struct measurement m;

bool updateVisuals = false;

//EEPROM
#define EEPROM_CHECK_ADDR 0
#define EEPROM_CHECK_VALUE 1
#define EEPROM_CONFIG_ADDR 10
#define EEPROM_CAL_ADDR 100

//timer
uint32_t m_timer = 0;

void setup() {

  Serial.begin(115200);
  Serial.setTimeout(1000);

  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();

  //getData();
  Serial.println("\nMessgerät - 101023.1");
  cal1.I_OFFSET = ACSCal(I_PIN);

  Serial.println("Bereit.");
  Serial.println("t\tmtype\tftype\tf in Hz\tU in V\tI in A");
  Serial.println("----------------------------------------------");
}

void loop() {

  timer();
  controls();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > config.MEASUREMENT_INTERVAL) {
    m_timer = now;
    m.time = now;
    if (m.m_type == U) {
      m.current_U = getVoltage(U_PIN);
    } else {
      m.freq = getFreq(I_PIN);
      m.current_I = getCurrent(I_PIN);
    }
    updateVisuals = true;
  }

  if (updateVisuals) {
    updateVisuals = false;
    updateDisplay();
    serialInfoTab();
    //serialInfoBlock();
  }
}

void controls() {

  if (!digitalRead(BTN1_PIN) && !btn1Lock) {
    btn1Lock = true;
    return;
  } else if (digitalRead(BTN1_PIN) && btn1Lock) {
    btn1Lock = false;
    if (m.m_type == U) {
      m.m_type = I;
      m.current_U = 0;
    } else {
      m.m_type = U;
      m.current_I = 0;
    }
    updateVisuals = true;
    return;
  }

  if (!digitalRead(BTN2_PIN) && !btn2Lock) {
    btn2Lock = true;
    return;
  } else if (digitalRead(BTN2_PIN) && btn2Lock) {
    btn2Lock = false;
    if (m.m_type == I) {
      cal1.I_OFFSET = ACSCal(I_PIN);
    } else {
      if (m.f_type == DC) {
        m.f_type = DAC;
      } else if (m.f_type == DAC) {
        m.f_type = AC;
      } else {
        m.f_type = DC;
        m.freq = 0.0f;
      }
    }
    updateVisuals = true;
    return;
  }
}

void getData() {

  if (EEPROM.read(EEPROM_CHECK_ADDR) != EEPROM_CHECK_VALUE) {
    EEPROM.put(EEPROM_CHECK_ADDR, EEPROM_CHECK_VALUE);
    EEPROM.put(EEPROM_CONFIG_ADDR, config);
    EEPROM.put(EEPROM_CAL_ADDR, cal1);
    Serial.println("Daten initial gespeichert.");
  } else {
    EEPROM.get(EEPROM_CONFIG_ADDR, config);
    EEPROM.get(EEPROM_CAL_ADDR, cal1);
    Serial.println("Daten geladen.");
  }
}
