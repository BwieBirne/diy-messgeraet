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

int8_t startAPandServer() {

  Serial.println("Access Point starten...");
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Kommunikation mit WiFi-Modul fehlgeschlagen!");
    return -1;
  }

  String fwv = WiFi.firmwareVersion();
  if (fwv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("WiFi-Modul Firmware kann aktualisiert werden!");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  WiFiStatus = WiFi.beginAP(NETWORK_SSID, NETWORK_PASS);
  if (WiFiStatus != WL_AP_LISTENING) {
    Serial.println("Access Point konnte nicht gestartet werden!");
    return -1;
  }

  Serial.println("Server starten...");
  server.begin();

  return 0;
}