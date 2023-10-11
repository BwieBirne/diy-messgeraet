
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

  return (0.0f);
}

int8_t senCal(const uint8_t mPin, struct configuration *conf, struct calibration *cal) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;

  sensorRead(mPin, &min, &max, &avg, conf->STD_PERIODDURATION, conf->MEASUREMENT_ITR);

  return 0;
}
