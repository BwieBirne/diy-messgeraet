
#define DISPLAY_CHAR_LENGTH 10

void updateDisplay() {

  displayUpdate = false;
  //ohne clear sieht schöner aus, aber vielleicht brauchen wir es, wenn es Anzeigebugs gibt
  //ssd1306_clearScreen();

  const char U_char[DISPLAY_CHAR_LENGTH];
  const char I_char[DISPLAY_CHAR_LENGTH];

  dtostrf(current_U, 6, 2, U_char);
  dtostrf(current_I, 6, 2, I_char);

  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_printFixed(0,  8, "U in V: ", STYLE_NORMAL);
  ssd1306_printFixed(0, 24, "I in A: ", STYLE_NORMAL);
  ssd1306_printFixed(60,  8, U_char, STYLE_BOLD);
  ssd1306_printFixed(60, 24, I_char, STYLE_BOLD);

  if (m_type == U) {
    ssd1306_printFixed(0,  40, "m: U", STYLE_NORMAL);
  } else {
    ssd1306_printFixed(0,  40, "m: I", STYLE_NORMAL);
  }

  if (f_type == AC) {
      ssd1306_printFixed(40,  40, "f: AC", STYLE_NORMAL);
  } else if (f_type == DAC) {
      ssd1306_printFixed(40,  40, "f: DAC", STYLE_NORMAL);
  } else {
      ssd1306_printFixed(40,  40, "f: DC", STYLE_NORMAL);
  }

}