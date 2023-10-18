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

    } else if (statusInt == 111) {

      Serial.println(statusInt);
      Serial.write((byte*)&msm, sizeof(measurement));
      Serial.println();

    } else if (statusInt == 121) {

      Serial.println(statusInt);
      Serial.readBytes((byte*)&config, sizeof(configuration));
      Serial.readBytes((byte*)&calU, sizeof(calU));
      Serial.readBytes((byte*)&calI, sizeof(calI));
      //EEPROM.put(EEPROM_CONFIG_ADDR, config);
      //EEPROM.put(EEPROM_CALU_ADDR, calU);
      //EEPROM.put(EEPROM_CALI_ADDR, calI);
    }
  }
}