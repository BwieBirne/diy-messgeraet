
void sensorRead(const uint8_t mPin, int16_t *min, int16_t *max, int16_t *avg, uint16_t periodduration, const uint8_t itr) {

  uint32_t minSum = 0;
  uint32_t maxSum = 0;
  uint32_t avgSum = 0;

  for (uint8_t i = 0; i < itr; i++) {

    uint16_t count = 0;
    int16_t newMin = 1023;
    int16_t newMax = 0;
    uint32_t newAvg = 0;

    uint32_t start = micros();
    while (micros() - start < periodduration) {
      uint16_t value = analogRead(mPin);
      if (value < newMin) newMin = value;
      if (value > newMax) newMax = value;
      newAvg += value;
      count++;
    }
    newAvg /= count;

    minSum += newMin;
    maxSum += newMax;
    avgSum += newAvg;
  }

  *min = minSum / itr;
  *max = maxSum / itr;
  *avg = avgSum / itr;
}


float getVoltage(const uint8_t mPin, struct configuration *conf, struct calibration *cal) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;

  sensorRead(mPin, &min, &max, &avg, conf->STD_PERIODDURATION, conf->MEASUREMENT_ITR);

  if (!avg) return (0.0f);

  min -= min * cal->U_CORRECTION_FACTOR - cal->U_CORRECTION_NORM;
  max -= max * cal->U_CORRECTION_FACTOR - cal->U_CORRECTION_NORM;
  avg -= avg * cal->U_CORRECTION_FACTOR - cal->U_CORRECTION_NORM;

  //serialMinMaxAvg(min, max, avg);

  if (f_type != DC) return ((min + ((max - min) / (SQRT2))) / (cal->U_DIVIDER));

  return (avg / cal->U_DIVIDER);
}


float getCurrent(const uint8_t mPin, struct configuration *conf, struct calibration *cal) {

  float Umess = getVoltage(mPin, conf, cal);

  return (Umess / cal->I_RESISTANCE);
}

float getFreq(const uint8_t mPin, struct configuration *conf, struct calibration *cal) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;

  sensorRead(mPin, &min, &max, &avg, conf->STD_PERIODDURATION, conf->MEASUREMENT_ITR);

  if (max - min > conf->DAC_THRESHOLD) return (0.0f);

  //Frequenz bestimmen

  return (50.0f);
}

int8_t senCal(const uint8_t mPin, struct configuration *conf, struct calibration *cal) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg1 = 0;

  Serial.println("Die Kalibrierung muss unter Gleichstrom erfolgen!");
  Serial.print("U auf ");
  Serial.print(conf->CAL_POINTS[0]);
  Serial.println(" V stellen und anschließen BTN1 drücken.");

  while (digitalRead(conf->BTN1_PIN));

  sensorRead(mPin, &min, &max, &avg1, conf->STD_PERIODDURATION, conf->MEASUREMENT_ITR);

  if (max - min > conf->DAC_THRESHOLD) return 1;

  Serial.print("U auf ");
  Serial.print(conf->CAL_POINTS[1]);
  Serial.println(" V stellen und anschließen BTN1 drücken.");

  int16_t avg2 = 0;
  
  while (digitalRead(conf->BTN1_PIN));

  sensorRead(mPin, &min, &max, &avg2, conf->STD_PERIODDURATION, conf->MEASUREMENT_ITR);

  if (max - min > 12) return 1;

  //Berechnung U_DIVIDER
  Serial.println((avg2-avg1)/(conf->CAL_POINTS[1]-conf->CAL_POINTS[0]));

  //Berechnung U_CORRECTION_FACTOR und U_CORRECTION_NORM

  //while(1);

  return 0;
}
