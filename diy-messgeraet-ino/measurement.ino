
void sensorRead(const int mPin, int16_t* min, int16_t* max, int16_t* avg, uint16_t wavelength, const uint8_t itr) {

  uint32_t minSum = 0;
  uint32_t maxSum = 0;
  uint32_t avgSum = 0;

  for (uint8_t i = 0; i < itr; i++) {

    uint16_t count = 0;
    int16_t newMin = 1023;
    int16_t newMax = 0;
    uint32_t newAvg = 0;

    uint32_t start = micros();
    while (micros() - start < wavelength) {
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


float getVoltage(const int mPin) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;

  sensorRead(mPin, &min, &max, &avg, m.wavelength, config.WAVE_ITR);

  if (!avg) return (0.0f);

  min -= min * cal1.U_CORRECTION_FACTOR - cal1.U_CORRECTION_NORM;
  max -= max * cal1.U_CORRECTION_FACTOR - cal1.U_CORRECTION_NORM;
  avg -= avg * cal1.U_CORRECTION_FACTOR - cal1.U_CORRECTION_NORM;

  serialMinMaxAvg(min, max, avg);

  if (m.f_type != DC) return ((min + ((max - min) / (SQRT2))) / (cal1.U_DIVIDER));

  return (avg / cal1.U_DIVIDER);
}


float getCurrent(const int mPin) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;

  sensorRead(mPin, &min, &max, &avg, m.wavelength, config.WAVE_ITR);

  min -= 512 + cal1.I_OFFSET;
  max -= 512 + cal1.I_OFFSET;
  avg -= 512 + cal1.I_OFFSET;

  serialMinMaxAvg(min, max, avg);

  if (m.f_type != DC) {
    if (abs(max) > abs(min)) return ((min + ((max - min) / (SQRT2))) / (cal1.I_DIVIDER));
    else return ((max + ((min - max) / (SQRT2))) / (cal1.I_DIVIDER));
  }

  return (avg / cal1.I_DIVIDER);
}

float getFreq(const int mPin) {

  int16_t min = 1023;
  int16_t max = 0;
  int16_t avg = 0;
  uint16_t timeOut = 1e6 / config.MIN_FREQ;

  sensorRead(mPin, &min, &max, &avg, timeOut, 1);

  serialMinMaxAvg(min, max, avg);

  if ((max - min) < cal1.FREQ_DC_BOUND) {
    m.f_type = DC;
    return (0.0f);
  } else if ((max > (512 - cal1.FREQ_DC_BOUND) && min > (512 - cal1.FREQ_DC_BOUND)) || (max < (512 + cal1.FREQ_DC_BOUND) && min < (512 + cal1.FREQ_DC_BOUND))) {
    m.f_type = DAC;
  } else {
    m.f_type = AC;
  }

  //das geht sicher noch besser
  //---------------------------

  uint16_t Q1 = (3 * min + max) / 4;
  uint16_t Q3 = (min + 3 * max) / 4;

  Serial.print("Q1: ");
  Serial.println(Q1);
  Serial.print("Q3: ");
  Serial.println(Q3);

  timeOut *= config.FREQ_ITR;
  uint32_t start = micros();

  while ((analogRead(mPin) > Q1) && ((micros() - start) < timeOut))
    ;
  while ((analogRead(mPin) <= Q3) && ((micros() - start) < timeOut))
    ;

  start = micros();
  for (uint16_t i = 0; i < config.FREQ_ITR; i++) {
    while ((analogRead(mPin) > Q1) && ((micros() - start) < timeOut))
      ;
    while ((analogRead(mPin) <= Q3) && ((micros() - start) < timeOut))
      ;
  }
  uint32_t stop = micros();

  //---------------------------

  float wavelength = stop - start;
  //m.wavelength = wavelength / config.FREQ_ITR;
  return (config.FREQ_ITR * 1e6 / wavelength);
}

int8_t ACSCal(const int mPin) {

  //setzt voraus, dass I zum Zeitpunkt der Kalibrierung null ist
  Serial.println("Kalibrierung...");
  delay(1000);

  uint32_t sum = 0;

  for (uint16_t i = 0; i < config.CAL_ITR; i++) {
    sum += analogRead(mPin);
  }

  Serial.print("I_OFFSET: ");
  Serial.println((int32_t)(sum / config.CAL_ITR) - 512);

  return ((sum / config.CAL_ITR) - 512);
}
