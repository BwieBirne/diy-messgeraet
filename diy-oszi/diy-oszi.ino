
#define BAUD_RATE 115200
#define PIN A0
#define INPUT_VOLTAGE 3.3f  //in V
#define AR_RESOLUTION 10
#define ADC_RESOLUTION 1023
#define VOLTAGE_DIV (float)(ADC_RESOLUTION / INPUT_VOLTAGE)
#define FAST_AR_MODE true

#define DELAY 20

void setup() {

  Serial.begin(BAUD_RATE);

  if (FAST_AR_MODE) {

    while (ADC->STATUS.bit.SYNCBUSY == 1);

    ADC->CTRLB.reg &= 0b1111100011111111;                                     // mask PRESCALER bits
    ADC->CTRLB.reg |= ADC_CTRLB_PRESCALER_DIV64;                              // divide Clock by 64
    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 | ADC_AVGCTRL_ADJRES(0x00ul);  // adjusting result by 0
    ADC->SAMPCTRL.reg = 0x00;                                                 // sampling Time Length = 0
  }

  pinMode(PIN, INPUT);
  while (!Serial);

}

void loop() {
  
  delayMicroseconds(DELAY);
  Serial.println(analogRead(PIN) / VOLTAGE_DIV);
  //Serial.println(fastAnalogRead(PIN) / VOLTAGE_DIV);
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
