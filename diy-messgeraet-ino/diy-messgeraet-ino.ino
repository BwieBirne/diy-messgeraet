//Nano 33 IoT
//includes
#include <avr/dtostrf.h>
#include "ssd1306.h"  //Alexey Dynda
//#include <EEPROM.h>
#include <WiFiNINA.h>
//#include <ArduinoBLE.h>

#define CONFIG_MODE false
#define BAUD_RATE 115200
#define PINS_COUNT 3
#define INPUT_VOLTAGE 3.3f  //in V
#define AR_RESOLUTION 10
#define ADC_RESOLUTION 1023
#define VOLTAGE_DIV (float)(ADC_RESOLUTION / INPUT_VOLTAGE)
#define FAST_AR_MODE true

//WIFI - min 8 digits
#define NETWORK_SSID "SSIDSSID"
#define NETWORK_PASS "12345678"
#define MAX_BUFFER 10

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
  const uint8_t BTN1_PIN = 4;
  const uint8_t BTN2_PIN = 7;
  const float CAL_POINTS[2] = { 1.0f, 5.0f };  //in V
  const uint16_t MEASUREMENT_INTERVAL = 1000;  //in milliseconds
  const uint8_t MEASUREMENT_ITR = 2;
  const uint8_t FREQ_ITR = 2;
  const uint8_t MIN_FREQ = 50;                //in Hz
  const uint32_t STD_PERIODDURATION = 20000;  //in microseconds - 50Hz
  const uint8_t DAC_THRESHOLD = 8;
};

//calibration
typedef struct calibration {
  float U_DIVIDER = 60.75f;
  float U_OFFSET = -0.037f;
  float I_RESISTANCE = 5.0f;  //in Ohm - Messwiderstand muss noch ermittelt werden
};

//measurement
typedef struct measurement {
  uint32_t time = 0;  //in milliseconds
  bool voltage = true;
  float current_U[PINS_COUNT] = { 0.0f, 0.0f, 0.0f };  //in V
  float current_I[PINS_COUNT] = { 0.0f, 0.0f, 0.0f };  //in A
  float current_f[2] = { 0.0f, 0.0f };                 //in Hz - 0.0f is DC
};

typedef struct measurementReduced {
  uint32_t time = 0;  //in milliseconds
  bool voltage = true;
  float values[PINS_COUNT] = { 0.0f, 0.0f, 0.0f };  //in V or A
  //float f = 0.0f;                                   //in Hz - 0.0f is DC
};

//constants
#define SQRT2 1.4142

//timer
uint32_t m_timer = 0;  //in milliseconds

struct configuration config;
struct calibration calU[PINS_COUNT];
struct calibration calI[PINS_COUNT];
struct measurement msm;

bool measureU = true;
bool DC = true;
bool updateVisuals = false;

int WiFiStatus = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {

  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1000);

  if (FAST_AR_MODE) {

    while (ADC->STATUS.bit.SYNCBUSY == 1);

    ADC->CTRLB.reg &= 0b1111100011111111;                                     // mask PRESCALER bits
    ADC->CTRLB.reg |= ADC_CTRLB_PRESCALER_DIV64;                              // divide Clock by 64
    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 | ADC_AVGCTRL_ADJRES(0x00ul);  // adjusting result by 0
    ADC->SAMPCTRL.reg = 0x00;                                                 // sampling Time Length = 0
  }

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    pinMode(config.U_PINS[i], INPUT);
    pinMode(config.I_PINS[i], INPUT);
  }
  pinMode(config.BTN1_PIN, INPUT_PULLUP);
  pinMode(config.BTN2_PIN, INPUT_PULLUP);

  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();

  //getData();
  while (!Serial);
  Serial.println("\nMessgerät - 131123.1");

  if (CONFIG_MODE) {
    //hier Konfiguration für Kalibrierung anpassen
    if (senCal(config.U_PINS[0], &config, &calU[0])) {
      Serial.println("Sensor konnte nicht kalibriert werden!");
    } else {
      Serial.println("Kalibrierung erfolgreich!");
      //EEPROM.put(EEPROM_CALU_ADDR, calU);
      //EEPROM.put(EEPROM_CALI_ADDR, calI);
      while (1);
    }
  }

  if (startAPandServer()) {
    Serial.println("Access Point und Server konnten nicht gestartet werden!");
  } else {
    Serial.println("Access Point und Server gestartet!");
    printWiFiStatus();
  }

  delay(500);

  Serial.println("Bereit.");

  printTabHeadSerial();
}

void loop() {

  //webserver();
  timer();
  //controls();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > config.MEASUREMENT_INTERVAL) {
    m_timer = now;
    msm.time = now;
    if (measureU) {
      msm.voltage = true;
      msm.current_f[0] = getFreq(config.U_PINS[0], &config, &calU[0]);
      for (uint8_t i = 0; i < PINS_COUNT; i++) {
        msm.current_U[i] = getVoltage(config.U_PINS[i], &config, &calU[i]);
      }
    } else {
      msm.voltage = false;
      msm.current_f[1] = getFreq(config.I_PINS[0], &config, &calI[0]);
      for (uint8_t i = 0; i < PINS_COUNT; i++) {
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
    measureU = !measureU;
    updateVisuals = true;
    return;
  }

  if (!digitalRead(config.BTN2_PIN) && !btn2Lock) {
    btn2Lock = true;
    return;
  } else if (digitalRead(config.BTN2_PIN) && btn2Lock) {
    btn2Lock = false;
    DC = !DC;
    updateVisuals = true;
    return;
  }
}

uint16_t fastAnalogRead(const uint8_t mPin) {

  ADC->CTRLA.bit.ENABLE = 0;

  while (ADC->STATUS.bit.SYNCBUSY == 1);

  int CTRLBoriginal = ADC->CTRLB.reg;
  int AVGCTRLoriginal = ADC->AVGCTRL.reg;
  int SAMPCTRLoriginal = ADC->SAMPCTRL.reg;

  ADC->CTRLB.reg &= 0b1111100011111111;
  ADC->CTRLB.reg |= ADC_CTRLB_PRESCALER_DIV64;
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 | ADC_AVGCTRL_ADJRES(0x00ul);
  ADC->SAMPCTRL.reg = 0x00;

  ADC->CTRLA.bit.ENABLE = 1;
  while (ADC->STATUS.bit.SYNCBUSY == 1);

  uint16_t adc = analogRead(mPin);

  ADC->CTRLB.reg = CTRLBoriginal;
  ADC->AVGCTRL.reg = AVGCTRLoriginal;
  ADC->SAMPCTRL.reg = SAMPCTRLoriginal;

  return adc;
}

/*
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
*/
