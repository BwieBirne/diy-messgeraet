//Schnittstelle für Debugging, Datenübertragung und -speicherung

void serialEvent() {

  while (Serial.available()) {

    while (Serial.peek() == 10 || Serial.peek() == 13) {
      Serial.read();
    }

    int statusInt = Serial.parseInt();

    if (statusInt == 100) {
      Serial.println(statusInt);
      //Serial.println(config);
      //Serial.println(cal1);
    } else if (statusInt == 101) {
      Serial.println(statusInt);
      Serial.println(millis());
      Serial.println(m_type);
      Serial.println(f_type);
      Serial.println(freq);
      Serial.println(current_U);
      Serial.println(current_I);
    } else if (statusInt == 110) {
      
    }
  }
}