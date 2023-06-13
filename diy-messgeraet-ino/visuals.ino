
#define DISPLAY_CHAR_LENGTH 10

void updateDisplay() {

  displayUpdate = false;
  //ssd1306_clearScreen();

  const char U_char[DISPLAY_CHAR_LENGTH];
  const char I_char[DISPLAY_CHAR_LENGTH];

  dtostrf(current_U, 6, 2, U_char);
  dtostrf(current_I, 6, 2, I_char);

  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_printFixed(0,  8, "U in V: ", STYLE_NORMAL);
  ssd1306_printFixed(0, 24, "I in A: ", STYLE_NORMAL);
  ssd1306_printFixed(0,  40, "m: ", STYLE_NORMAL);
  ssd1306_printFixed(40, 40, "f: ", STYLE_NORMAL);
  ssd1306_printFixed(60,  8, U_char, STYLE_BOLD);
  ssd1306_printFixed(60, 24, I_char, STYLE_BOLD);

  if(m_type == U) {
    ssd1306_printFixed(20,  40, "U", STYLE_NORMAL);
  } else {
    ssd1306_printFixed(20,  40, "I", STYLE_NORMAL);
  }

  /*switch(f_type) {
     case AC:
      ssd1306_printFixed(60,  40, "AC", STYLE_NORMAL);
    case DAC:
      ssd1306_printFixed(60,  40, "DAC", STYLE_NORMAL);
     case DC:
      ssd1306_printFixed(60,  40, "DC", STYLE_NORMAL);

  }*/

}