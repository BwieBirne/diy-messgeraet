
//includes
#include "math.h"
#include <EEPROM.h>

//constants
#define RECORD_MODE false
#define BAUD_RATE 115200
#define PIN A0
#define VOLTAGE_DIV 204.6f
#define MEASUREMENT_INTERVAL 200  //in milliseconds
#define ARRAY_LENGTH 4

//EEPROM
#define EEPROM_CHECK_ADDR 0
#define EEPROM_CHECK_VALUE 1
#define EEPROM_ARRAY_ADDR 10

typedef struct configuration {
  uint8_t mode = 2;
  uint16_t freq = 50;  //in Hz
  float VDC = 0.0f;    //in V - max 5.0V
  float Vpp = 4.0f;    //in V - max 5.0V
  float V0 = 0.0f;     //in V - max 5.0V
};

uint8_t waveMode;
uint32_t periodduration;   //in microseconds
uint32_t periodduration2;  //in microseconds
uint16_t minValue;
uint16_t maxValue;
uint16_t V0Value;

uint32_t start;  //in microseconds
uint32_t stop;   //in microseconds

//timer
uint32_t m_timer = 0;  //in milliseconds

struct configuration config;

uint16_t mArray[ARRAY_LENGTH];

void initArray() {

  for (uint16_t i = 0; i < ARRAY_LENGTH; i++) {
    mArray[i] = 0;
  }
}

void update(struct configuration *conf) {

  waveMode = conf->mode;
  periodduration = 1e6 / conf->freq;
  periodduration2 = (1e6 / conf->freq) / 2;
  minValue = conf->VDC * VOLTAGE_DIV;
  maxValue = minValue + (conf->Vpp * VOLTAGE_DIV);
  V0Value = conf->V0 * VOLTAGE_DIV;
}

void setup() {

  Serial.begin(BAUD_RATE);
  pinMode(PIN, OUTPUT);
  initArray();
  update(&config);

  Serial.println("\nFrequenzgenerator - 181023.1");

  delay(500);

  Serial.println("Bereit.");

  if (RECORD_MODE) {
    if (record()) {
      Serial.println("Aufnahme fehlgeschlagen - Array zu klein!");
      initArray();
    } else {
      Serial.println("Aufnahme erfolgreich!");
      //EEPROM.put(EEPROM_ARRAY_ADDR, mArray);
      while(1);
    }
  }

  Serial.println("-------------");
}

void loop() {

  switch (waveMode) {
    case 0:
      DC();
      break;
    case 1:
      sine();
      break;
    case 2:
      sineDC();
      break;
    case 3:
      rect();
      break;
    case 4:
      rectDgt();
      break;
    case 5:
      tri();
      break;
    case 6:
      ramp();
      break;
    case 7:
      rando();
      break;
    case 8:
      costum();
      break;
  }

  timer();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > MEASUREMENT_INTERVAL) {

    m_timer = now;

    Serial.print("t: ");
    Serial.print(millis());
    Serial.println(" ms");
    Serial.print("T: ");
    Serial.print((stop - start) / 1e3);
    Serial.println(" ms");
    Serial.print("f: ");
    Serial.print(1e6 / (stop - start));
    Serial.println(" Hz");
    Serial.println("-------------");
  }
}
