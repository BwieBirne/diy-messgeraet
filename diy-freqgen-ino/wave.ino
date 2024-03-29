
void DC() {

  analogWrite(OUT_PIN, minValue);
  //delay(1000);
}

void sine() {

  start = micros();

  while ((micros() - start) < periodduration) {
    analogWrite(OUT_PIN, (int16_t)(minValue + (sin((((micros() - start) / (float)periodduration)) * 2 * M_PI) * maxValue)));
  }
  
  stop = micros();
}

void sineDC() {

  start = micros();

  while ((micros() - start) < periodduration) {
    analogWrite(OUT_PIN, (int16_t)(minValue + (sin((((micros() - start) / (float)periodduration)) * M_PI) * maxValue)));
  }

  stop = micros();
}

void rect() {

  start = micros();

  analogWrite(OUT_PIN, minValue);
  delayMicroseconds(periodduration2);
  analogWrite(OUT_PIN, maxValue);
  delayMicroseconds(periodduration2);

  stop = micros();
}

void rectDgt() {

  start = micros();

  digitalWrite(OUT_PIN, LOW);
  delayMicroseconds(periodduration2);
  digitalWrite(OUT_PIN, HIGH);
  delayMicroseconds(periodduration2);

  stop = micros();
}

void tri() {

  start = micros();

  while ((micros() - start) < periodduration2) {
    analogWrite(OUT_PIN, (int16_t)(minValue + (((micros() - start) / (float)periodduration2) * maxValue)));
  }
  uint32_t start2 = micros();
  while ((micros() - start2) < periodduration2) {
    analogWrite(OUT_PIN, (int16_t)(minValue + ((1 - ((micros() - start2) / (float)periodduration2)) * maxValue)));
  }

  stop = micros();
}

void ramp() {

  start = micros();

  while ((micros() - start) < periodduration) {
    analogWrite(OUT_PIN, (uint16_t)(minValue + (((micros() - start) / (float)periodduration) * maxValue)));
  }

  stop = micros();
}

void rando() {

  start = micros();

  while ((micros() - start) < periodduration) {
    analogWrite(OUT_PIN, random(minValue, maxValue));
  }

  stop = micros();
}

void costum() {

  start = micros();

  uint16_t i = 0;
  while ((mArray[i] != -1) && (i < ARRAY_LENGTH)) {
    analogWrite(OUT_PIN, mArray[i]);
    i++;
  }

  stop = micros();
}

int8_t record() {

  start = micros();

  uint16_t i = 0;
  while (((micros() - start) < periodduration) && (i < ARRAY_LENGTH)) {
    mArray[i] = analogRead(OUT_PIN);
    i++;
  }

  stop = micros();

  if (i == ARRAY_LENGTH) {
    return -1;
  } else {
    mArray[i] = -1;
    return 0;
  }
}