#define DISPLAY_CHAR_LENGTH 10

void printMeasurementDisplay(struct measurement *m) {

  //ohne clear sieht schöner aus, aber vielleicht brauchen wir es, wenn es Anzeigebugs gibt
  //ssd1306_clearScreen();

  char U_char[DISPLAY_CHAR_LENGTH];
  char I_char[DISPLAY_CHAR_LENGTH];
  char f_char[DISPLAY_CHAR_LENGTH];

  dtostrf(m->current_U[0], 6, 2, U_char);
  dtostrf(m->current_I[0], 6, 2, I_char);
  dtostrf(m->current_f[0], 6, 2, f_char);

  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_printFixed(0, 8, "U:", STYLE_NORMAL);
  ssd1306_printFixed(0, 24, "I:", STYLE_NORMAL);
  ssd1306_printFixed(0, 40, "f:", STYLE_NORMAL);
  ssd1306_printFixed(16, 8, U_char, STYLE_BOLD);
  ssd1306_printFixed(16, 24, I_char, STYLE_BOLD);
  ssd1306_printFixed(16, 40, f_char, STYLE_BOLD);
  ssd1306_printFixed(56, 8, "V", STYLE_NORMAL);
  ssd1306_printFixed(56, 24, "A", STYLE_NORMAL);
  ssd1306_printFixed(56, 40, "Hz", STYLE_NORMAL);
}

void printMeasurementBlockSerial(struct measurement *m) {

  Serial.print("t: ");
  Serial.print(m->time);
  Serial.println(" ms");

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print("U");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(m->current_U[i]);
    Serial.println(" V");
  }

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print("I");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(m->current_I[i]);
    Serial.println(" A");
  }

  Serial.print("fU: ");
  Serial.print(m->current_f[0]);
  Serial.println(" Hz");
  Serial.print("fI: ");
  Serial.print(m->current_f[1]);
  Serial.println(" Hz");

  Serial.println("----------------------------------------------------------------------");
}

void printTabHeadSerial() {

  Serial.print("t(ms)\t");

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print("U");
    Serial.print(i);
    Serial.print("(V)\t");
  }

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print("I");
    Serial.print(i);
    Serial.print("(A)\t");
  }

  Serial.print("fU(Hz)\t");
  Serial.print("fI(Hz)\t");

  Serial.println();

  Serial.println("----------------------------------------------------------------------");
}

void printMeasurementTabSerial(struct measurement *m) {

  Serial.print(m->time);
  Serial.print("\t");

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print(m->current_U[i]);
    Serial.print("\t");
  }

  for (uint8_t i = 0; i < PINS_COUNT; i++) {
    Serial.print(m->current_I[i]);
    Serial.print("\t");
  }

  Serial.print(m->current_f[0]);
  Serial.print("\t");
  Serial.print(m->current_f[1]);
  Serial.print("\t");

  Serial.println();
}

void printWiFiStatus() {

  Serial.print("SSID: ");
  //Serial.println(NETWORK_SSID);
  Serial.println(WiFi.SSID());
  Serial.print("PASS: "); 
  Serial.println(NETWORK_PASS);

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}

void printMinMaxAvgSerial(int min, int max, int avg) {

  Serial.print("Min: ");
  Serial.println(min);
  Serial.print("Max: ");
  Serial.println(max);
  Serial.print("Avg: ");
  Serial.println(avg);
}
