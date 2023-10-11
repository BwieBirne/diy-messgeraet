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
      Serial.println();
      Serial.write((byte*)&calU, sizeof(calU));
      Serial.println();
      Serial.write((byte*)&calI, sizeof(calI));
      Serial.println();
    } else if (statusInt == 110) {

      Serial.println(statusInt);
      Serial.write((byte*)&msm, sizeof(measurement));
      Serial.println();
    } else if (statusInt == 111) {

      Serial.println(statusInt);
      //Rohwerte aus Speicher, unverarbeitet
      //Serial.println();
    } else if (statusInt == 120) {

      struct configuration tempConfig;
      struct calibration tempCalU[PINS_COUNT];
      struct calibration tempCalI[PINS_COUNT];
      Serial.println(statusInt);
      uint8_t configBytes = Serial.readBytes((byte*)&config, sizeof(configuration));
      uint8_t calUBytes = Serial.readBytes((byte*)&calU, sizeof(calU));
      uint8_t calIBytes = Serial.readBytes((byte*)&calI, sizeof(calI));
      Serial.println(configBytes);
      //Serial.println(calBytes);
      //EEPROM speichern
    }
  }
}