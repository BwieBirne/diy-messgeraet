
float getVoltage(const int mPin) {

  uint16_t mArray[config.MEASUREMENT_ITR];
  uint32_t avg = 0;
  int16_t min = 1023;
  int16_t max = 0;

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    mArray[i] = analogRead(mPin);
    delayMicroseconds(config.MEASUREMENT_DELAY);
  }

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  max -= cal1.U_ERROR;
  min += cal1.U_ERROR;

  if (m.f_type != DC) return ((min + ((max - min) / (SQRT2))) / (cal1.U_DIVIDER));

  avg = avg / config.MEASUREMENT_ITR;

  return (avg / cal1.U_DIVIDER);
}


float getCurrent(const int mPin) {

  uint16_t mArray[config.MEASUREMENT_ITR];
  int32_t avg = 0;
  int16_t min = 1023;
  int16_t max = 0;

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    mArray[i] = analogRead(mPin);
    delayMicroseconds(config.MEASUREMENT_DELAY);
  }

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    mArray[i] -= 512 + cal1.I_OFFSET;
  }

  min -= 512 + cal1.I_OFFSET;
  max -= 512 + cal1.I_OFFSET;
  max -= cal1.I_ERROR;
  min += cal1.I_ERROR;

  if (m.f_type != DC) {
    if (abs(max) > abs(min)) return ((min + ((max - min) / (SQRT2))) / (cal1.I_DIVIDER));
    else return ((max + ((min - max) / (SQRT2))) / (cal1.I_DIVIDER));
  }

  avg = avg - ((long)config.MEASUREMENT_ITR * (512 + cal1.I_OFFSET));
  avg = avg / config.MEASUREMENT_ITR;

  return (avg / cal1.I_DIVIDER);
}

float getFreq(const int mPin) {

  uint16_t max = 0;
  uint16_t min = 0;
  max = min = analogRead(mPin);
  uint32_t timeOut = 1e6 / config.MIN_FREQ;

  uint32_t start = micros();
  while (micros() - start < timeOut) {
    uint16_t value = analogRead(mPin);
    if (value < min) min = value;
    if (value > max) max = value;
  }

  if ((max - min) < cal1.FREQ_DC_BOUND) {
    m.f_type = DC;
    return (0.0f);
  } else if ((max > (512 - cal1.FREQ_DC_BOUND) && min > (512 - cal1.FREQ_DC_BOUND)) || (max < (512 + cal1.FREQ_DC_BOUND) && min < (512 + cal1.FREQ_DC_BOUND))) {
    m.f_type = DAC;
  } else {
    m.f_type = AC;
  }

  uint16_t Q1 = (3 * min + max) / 4;
  uint16_t Q3 = (min + 3 * max) / 4;

  timeOut *= config.FREQ_ITR;
  start = micros();

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

  float wavelength = stop - start;
  return (config.FREQ_ITR * 1e6 / wavelength);
}

int8_t ACSCal(const int mPin) {

  //setzt voraus, dass I zum Zeitpunkt der Kalibrierung null ist
  Serial.println("Kalibrierung...");
  delay(1000);

  uint32_t sum = 0;

  for (uint16_t i = 0; i < config.MEASUREMENT_ITR; i++) {
    sum += analogRead(mPin);
    delayMicroseconds(1);
  }

  Serial.print("I_OFFSET: ");
  Serial.println((int32_t)(sum / config.MEASUREMENT_ITR) - 512);

  return ((sum / config.MEASUREMENT_ITR) - 512);
}
