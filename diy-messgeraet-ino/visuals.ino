#define DISPLAY_CHAR_LENGTH 10

void updateDisplay() {

  //ohne clear sieht schöner aus, aber vielleicht brauchen wir es, wenn es Anzeigebugs gibt
  //ssd1306_clearScreen();

  const char U_char[DISPLAY_CHAR_LENGTH];
  const char I_char[DISPLAY_CHAR_LENGTH];
  const char f_char[DISPLAY_CHAR_LENGTH];

  dtostrf(current_U, 6, 2, U_char);
  dtostrf(current_I, 6, 2, I_char);
  dtostrf(freq, 6, 2, f_char);

  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_printFixed(0, 8, "U in V: ", STYLE_NORMAL);
  ssd1306_printFixed(0, 24, "I in A: ", STYLE_NORMAL);
  ssd1306_printFixed(60, 8, U_char, STYLE_BOLD);
  ssd1306_printFixed(60, 24, I_char, STYLE_BOLD);

  ssd1306_printFixed(0, 40, "f in Hz:", STYLE_NORMAL);
  ssd1306_printFixed(60, 40, f_char, STYLE_BOLD);

  if (m_type == U) {
    ssd1306_printFixed(0, 56, "m: U", STYLE_NORMAL);
  } else {
    ssd1306_printFixed(0, 56, "m: I", STYLE_NORMAL);
  }

   if (f_type == AC) {
      ssd1306_printFixed(40,  56, "f: AC", STYLE_NORMAL);
  } else if (f_type == DAC) {
      ssd1306_printFixed(40,  56, "f: DAC", STYLE_NORMAL);
  } else {
      ssd1306_printFixed(40,  56, "f: DC", STYLE_NORMAL);
  }
}

void serialInfoBlock() {

  Serial.print("t: ");
  Serial.println(millis());
  Serial.print("mtype: ");
  if (m_type == U) {
    Serial.println("U");
  } else {
    Serial.println("I");
  }
  Serial.print("ftype: ");
  if (f_type == AC) {
    Serial.println("AC");
  } else if (f_type == DAC) {
    Serial.println("DAC");
  } else {
    Serial.println("DC");
  }
  Serial.print("f in Hz: ");
  Serial.println(freq);
  Serial.print("U in V: ");
  Serial.println(current_U);
  Serial.print("I in A: ");
  Serial.println(current_I);
  Serial.println("----------------------------------------------");
}

void serialInfoTab() {

  Serial.print(millis());
  Serial.print("\t");
  if (m_type == U) {
    Serial.print("U\t");
  } else {
    Serial.print("I\t");
  }
  if (f_type == AC) {
    Serial.print("AC\t");
  } else if (f_type == DAC) {
    Serial.print("DAC\t");
  } else {
    Serial.print("DC\t");
  }
  Serial.print(freq);
  Serial.print("\t");
  Serial.print(current_U);
  Serial.print("\t");
  Serial.println(current_I);
}
