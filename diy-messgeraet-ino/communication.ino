//Schnittstelle für Debugging, Datenübertragung und -speicherung

void serialEvent() {

  while (Serial.available()) {

    while (Serial.peek() == 10 || Serial.peek() == 13) {
      Serial.read();
    }

    int statusInt = Serial.parseInt();

    if (statusInt == 100) {
      Serial.println(statusInt);
    } else if (statusInt == 101) {
      Serial.println(statusInt);
      Serial.println(1);
    }
  }
}