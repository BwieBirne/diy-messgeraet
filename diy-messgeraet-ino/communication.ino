//Schnittstelle für Debugging, Datenübertragung und -speicherung

void serialEvent() {

  while (Serial.available()) {

    while (Serial.peek() == 10 || Serial.peek() == 13) {
      Serial.read();
    }

    int statusInt = Serial.parseInt();

    if (statusInt == 101) {

      Serial.println(statusInt);
      Serial.write((byte*)&config, sizeof(configuration));
      Serial.write((byte*)&cal1, sizeof(calibration));
      //Serial.println();
    } else if (statusInt == 110) {

      Serial.println(statusInt);
      Serial.write((byte*)&m, sizeof(measurement));
      //Serial.println();
    } else if (statusInt == 111) {

      Serial.println(statusInt);
      //Rohwerte aus Speicher, unverarbeitet
    } else if (statusInt == 120) {

      Serial.println(statusInt);
      uint8_t configBytes = Serial.readBytes((byte*)&config, sizeof(configuration));
      uint8_t calBytes = Serial.readBytes((byte*)&cal1, sizeof(calibration));
      Serial.println(configBytes);
      Serial.println(calBytes);
      //EEPROM speichern
    }
  }
}