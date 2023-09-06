
//Schnittstelle für Debugging und Datensammlung


void serialEvent() {

  while (Serial.available()) {

    while (Serial.peek() == 10 || Serial.peek() == 13) {
      Serial.read();
    }

    uint8_t statusInt = Serial.parseInt();

    if (statusInt == 100) {
      Serial.println(statusInt);
    } else if (statusInt == 101) {
      Serial.println(statusInt);
      Serial.println(1);
    }
  }
}