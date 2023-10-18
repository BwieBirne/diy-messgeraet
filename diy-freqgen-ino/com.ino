
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

    } else if (statusInt == 111) {

      Serial.println(statusInt);
      Serial.readBytes((byte*)&config, sizeof(configuration));
      update(&config);
    }
  }
}