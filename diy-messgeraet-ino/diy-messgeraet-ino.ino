// NOTIZEN
// WICHTIG: KEINE SWITCH-CASE-STATEMENTS VERWENDEN!

//includes
#include "ssd1306.h"  //Alexey Dynda
#include <EEPROM.h>
//#include <WiFiNINA.h>
//#include <ArduinoBLE.h>

//enums
enum frequency_type { DC,
                      DAC };
enum measurement_type { U,
                        I };

#define CONFIG_MODE false
#define PINS_COUNT 3
#define BAUD_RATE 115200

//EEPROM
#define EEPROM_CHECK_ADDR 0
#define EEPROM_CHECK_VALUE 1
#define EEPROM_CONFIG_ADDR 10
#define EEPROM_CALU_ADDR 100
#define EEPROM_CALI_ADDR 200

//locks
bool btn1Lock = false;
bool btn2Lock = false;

//configuration
typedef struct configuration {
  const uint8_t U_PINS[PINS_COUNT] = { A0, A0, A0 };
  const uint8_t I_PINS[PINS_COUNT] = { A3, A3, A3 };
  const uint8_t BTN1_PIN = 2;
  const uint8_t BTN2_PIN = 4;
  const float CAL_POINTS[2] = { 1.0f, 5.0f };  //in V
  const uint16_t MEASUREMENT_INTERVAL = 1000;  //milliseconds
  const uint8_t MEASUREMENT_ITR = 2;
  const uint8_t FREQ_ITR = 2;
  const uint16_t STD_PERIODDURATION = 20000;  //microseconds - 50Hz
};

//calibration
typedef struct calibration {
  float U_DIVIDER = 40.5f;
  float U_OFFSET = 0.0617f;
  float I_RESISTANCE = 4.9f;  //Ohm - Messwiderstand muss noch ermittelt werden
  uint8_t DAC_THRESHOLD = 20;
};

//measurement
typedef struct measurement {
  uint32_t time = 0;                                   //milliseconds
  float current_U[PINS_COUNT] = { 0.0f, 0.0f, 0.0f };  //in V
  float current_I[PINS_COUNT] = { 0.0f, 0.0f, 0.0f };  //in A
  float current_f[2] = { 0.0f, 0.0f };                 //in Hz - 0.0f is DC
};

//constants
#define SQRT2 1.4142

//timer+
uint32_t m_timer = 0;

enum measurement_type m_type = U;
enum frequency_type f_type = DC;

struct configuration config;
struct calibration calU[PINS_COUNT];
struct calibration calI[PINS_COUNT];
struct measurement msm;

bool updateVisuals = false;


void setup() {

  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1000);

  for (int i = 0; i < PINS_COUNT; i++) {
    pinMode(config.U_PINS[i], INPUT);
    pinMode(config.I_PINS[i], INPUT);
  }
  pinMode(config.BTN1_PIN, INPUT_PULLUP);
  pinMode(config.BTN2_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();

  //getData();
  Serial.println("\nMessgerät - 131023.1");

  if (CONFIG_MODE) {
    //hier Konfiguration für Kalibrierung anpassen
    if (senCal(config.U_PINS[0], &config, &calU[0])) {
      Serial.println("Sensor konnte nicht kalibriert werden!");
    } else {
      Serial.println("Kalibrierung erfolgreich!");
      //EEPROM.put(EEPROM_CALU_ADDR, calU);
      //EEPROM.put(EEPROM_CALI_ADDR, calI);
    }
  }

  Serial.println("Bereit.");

  printTabHeadSerial();
}

void loop() {

  timer();
  controls();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > config.MEASUREMENT_INTERVAL) {
    m_timer = now;
    msm.time = now;
    if (m_type == U) {
      msm.current_f[0] = getFreq(config.U_PINS[0], &config, &calU[0]);
      for (int i = 0; i < PINS_COUNT; i++) {
        msm.current_U[i] = getVoltage(config.U_PINS[i], &config, &calU[i]);
      }
    } else {
      msm.current_f[1] = getFreq(config.I_PINS[0], &config, &calI[0]);
      for (int i = 0; i < PINS_COUNT; i++) {
        msm.current_I[i] = getCurrent(config.I_PINS[i], &config, &calI[i]);
      }
    }
    updateVisuals = true;
  }

  if (updateVisuals) {
    updateVisuals = false;
    printMeasurementDisplay(&msm);
    //printMeasurementBlockSerial(&msm);
    printMeasurementTabSerial(&msm);
  }
}

void controls() {

  if (!digitalRead(config.BTN1_PIN) && !btn1Lock) {
    btn1Lock = true;
    return;
  } else if (digitalRead(config.BTN1_PIN) && btn1Lock) {
    btn1Lock = false;
    if (m_type == U) {
      m_type = I;
    } else {
      m_type = U;
    }
    updateVisuals = true;
    return;
  }

  if (!digitalRead(config.BTN2_PIN) && !btn2Lock) {
    btn2Lock = true;
    return;
  } else if (digitalRead(config.BTN2_PIN) && btn2Lock) {
    btn2Lock = false;
    if (f_type == DC) {
      f_type = DAC;
    } else {
      f_type = DC;
    }
    updateVisuals = true;
    return;
  }
}

void getData() {

  if (EEPROM.read(EEPROM_CHECK_ADDR) != EEPROM_CHECK_VALUE) {
    EEPROM.put(EEPROM_CHECK_ADDR, EEPROM_CHECK_VALUE);
    EEPROM.put(EEPROM_CONFIG_ADDR, config);
    EEPROM.put(EEPROM_CALU_ADDR, calU);
    EEPROM.put(EEPROM_CALI_ADDR, calI);
    Serial.println("Daten initial gespeichert.");
  } else {
    EEPROM.get(EEPROM_CONFIG_ADDR, config);
    EEPROM.get(EEPROM_CALU_ADDR, calU);
    EEPROM.get(EEPROM_CALI_ADDR, calI);
    Serial.println("Daten geladen.");
  }
}
