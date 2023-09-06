
float getVoltage(const int mPin) {

  //max gibt Spitzenwert
  //min gibt DC Offset bei DC/DAC
  //avg ist NICHT der Effektivwert!
  int16_t mArray[MEASUREMENT_ITR];
  uint32_t avg = 0;
  int16_t min = 1023;
  int16_t max = -1023;

  for (uint8_t i = 0; i < MEASUREMENT_ITR; i++) {

    mArray[i] = analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  for (uint8_t i = 0; i < MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  avg = avg / MEASUREMENT_ITR;

  return (avg / U_DIVIDER);
}


float getCurrent(const int mPin) {

  //max gibt Spitzenwert
  //min gibt DC Offset bei DC/DAC
  //avg ist NICHT der Effektivwert!
  int16_t mArray[MEASUREMENT_ITR];
  int32_t avg = 0;
  int16_t min = 1023;
  int16_t max = -1023;

  for (uint8_t i = 0; i < MEASUREMENT_ITR; i++) {

    mArray[i] = analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  for (uint8_t i = 0; i < MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  for (uint8_t i = 0; i < MEASUREMENT_ITR; i++) {
    mArray[i] -= I_OFFSET;
  }
  min -= I_OFFSET;
  max -= I_OFFSET;
  avg = avg - ((long)MEASUREMENT_ITR * I_OFFSET);

  avg = avg / MEASUREMENT_ITR;

  return (avg / I_DIVIDER);
}

void getftype(const int mPin) {

  uint16_t maximum = 0;
  uint16_t minimum = 0;
  maximum = minimum = analogRead(mPin);

  //  determine maxima
  uint32_t timeOut = round(1000000.0 / minimalFrequency);
  uint32_t start = micros();
  while (micros() - start < timeOut) {
    int value = _analogRead(_pin);
    if (value > maximum) maximum = value;
    if (value < minimum) minimum = value;
  }

  //  calculate quarter points
  //  using quarter points is less noise prone than using one single midpoint
  int Q1 = (3 * minimum + maximum) / 4;
  int Q3 = (minimum + 3 * maximum) / 4;

  //  10x passing Quantile points
  //  wait for the right moment to start
  //  to prevent endless loop a timeout is checked.
  timeOut *= 10;
  start = micros();
  //  casting to int to keep compiler happy.
  while ((int(_analogRead(_pin)) > Q1) && ((micros() - start) < timeOut))
    ;
  while ((int(_analogRead(_pin)) <= Q3) && ((micros() - start) < timeOut))
    ;
  start = micros();
  for (int i = 0; i < 10; i++) {
    while ((int(_analogRead(_pin)) > Q1) && ((micros() - start) < timeOut))
      ;
    while ((int(_analogRead(_pin)) <= Q3) && ((micros() - start) < timeOut))
      ;
  }
  uint32_t stop = micros();

  //  calculate frequency
  float wavelength = stop - start;
  float frequency = 1e7 / wavelength;
  if (_microsAdjust != 1.0) frequency *= _microsAdjust;
  return frequency;
}

uint16_t ACSCal(const int mPin) {

  //setzt voraus, dass I zum Zeitpunkt der Kalibrierung null ist

  uint32_t sum = 0;

  for (int8_t i = 0; i < MEASUREMENT_ITR; i++) {

    sum += analogRead(mPin);
    delayMicroseconds(1);
  }

  return (sum / MEASUREMENT_ITR);
}
