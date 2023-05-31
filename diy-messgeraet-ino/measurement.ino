
float measurement(const int mPin) {


  //max gibt Spitzenwert
  //min gibt DC Offset
  //avg ist NICHT der Effektivwert!!
  int mArray[MEASUREMENT_ITR];
  long int avg = 0;
  int min = 1023;
  int max = -1023;

  for (int i = 0; i < MEASUREMENT_ITR; i++) {

    mArray[i] = analogRead(mPin);
    delayMicroseconds(MEASUREMENT_DELAY);
  }

  for (int i = 0; i < MEASUREMENT_ITR; i++) {
    if (mArray[i] < min) min = mArray[i];
    if (mArray[i] > max) max = mArray[i];
    avg += mArray[i];
  }

  if (m_type == I) {
    for (int i = 0; i < MEASUREMENT_ITR; i++) {
      mArray[i] -= I_OFFSET;
    }
    min -= I_OFFSET;
    max -= I_OFFSET;
    avg = avg - (MEASUREMENT_ITR * I_OFFSET);
  }

  avg = avg / MEASUREMENT_ITR;

  if (max - min > avg) {
    //AC kann nur über I festgestellt werden!
    f_type = DAC;
  } else {
    f_type = DC;
  }

  switch (f_type) {
    case DC:
      avg = (min + ((max - min) / (SQRT2)));
    case DAC:
      avg = (min + ((max - min) / (SQRT2)));
    case AC:
      avg = (min + ((max - min) / (SQRT2)));
  }

  switch (m_type) {
    case U:
      return (avg / U_DIVIDER);
    case I:
      return ((I_OFFSET - avg) / I_DIVIDER);
  }
}