
float getVoltage(const int mPin) {

  //max gibt Spitzenwert
  //min gibt DC Offset bei DC/DAC
  //avg ist NICHT der Effektivwert!
  uint16_t mArray[MEASUREMENT_ITR];
  uint32_t avg = 0;
  int16_t min = 1023;
  int16_t max = 0;

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    mArray[i] = analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }
  max -= U_OFFSET;
  min += U_OFFSET;

  if (f_type != DC) return ((min + ((max - min) / (SQRT2))) / (U_DIVIDER));

  avg = avg / MEASUREMENT_ITR;

  return (avg / U_DIVIDER);
}


float getCurrent(const int mPin) {

  //max gibt Spitzenwert
  //min gibt DC Offset bei DC/DAC
  //avg ist NICHT der Effektivwert!
  uint16_t mArray[MEASUREMENT_ITR];
  int32_t avg = 0;
  int16_t min = 1023;
  int16_t max = 0;

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    mArray[i] = analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    mArray[i] -= I_OFFSET;
  }
  min -= I_OFFSET;
  max -= I_OFFSET;

  if (f_type != DC) {
    if (abs(max) > abs(min)) return ((min + ((max - min) / (SQRT2))) / (I_DIVIDER));
    else return ((max + ((min - max) / (SQRT2))) / (I_DIVIDER));
  }

  avg = avg - ((long)MEASUREMENT_ITR * I_OFFSET);
  avg = avg / MEASUREMENT_ITR;

  return (avg / I_DIVIDER);
}

void getFreq(const int mPin) {

  uint16_t max = 0;
  uint16_t min = 0;
  max = min = analogRead(mPin);
  uint32_t timeOut = 1000000 / MIN_FREQ;

  uint32_t start = micros();
  while (micros() - start < timeOut) {
    uint16_t value = analogRead(mPin);
    if (value < min) min = value;
    if (value > max) max = value;
  }

  if (max - min < FREQ_DC_BOUND) {
    f_type = DC;
    freq = 0.0;
    return;
  } else if (max > 512 - FREQ_DAC_BOUND && min > 512 - FREQ_DAC_BOUND || max < 512 + FREQ_DAC_BOUND && min < 512 + FREQ_DAC_BOUND) {
    f_type = DAC;
  } else {
    f_type = AC;
  }

  uint16_t Q1 = (3 * min + max) / 4;
  uint16_t Q3 = (min + 3 * max) / 4;

  timeOut *= FREQ_ITR;
  start = micros();

  while ((analogRead(mPin) > Q1) && ((micros() - start) < timeOut))
    ;
  while ((analogRead(mPin) <= Q3) && ((micros() - start) < timeOut))
    ;

  start = micros();
  for (uint16_t i = 0; i < FREQ_ITR; i++) {
    while ((analogRead(mPin) > Q1) && ((micros() - start) < timeOut))
      ;
    while ((analogRead(mPin) <= Q3) && ((micros() - start) < timeOut))
      ;
  }
  uint32_t stop = micros();

  float wavelength = stop - start;
  freq = FREQ_ITR * 1000000 / wavelength;
}

uint16_t ACSCal(const int mPin) {

  //setzt voraus, dass I zum Zeitpunkt der Kalibrierung null ist
  Serial.println("Kalibrierung...");

  uint32_t sum = 0;

  for (uint16_t i = 0; i < MEASUREMENT_ITR; i++) {
    sum += analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  return (sum / MEASUREMENT_ITR);
}
