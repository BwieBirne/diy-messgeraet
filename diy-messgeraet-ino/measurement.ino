
float measurement(const int mPin){

  int mArray[MEASUREMENT_ITR];

  for (int i = 0; i < MEASUREMENT_ITR; i++) {

    mArray[i] = analogRead(mPin);

  }

}