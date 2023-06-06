
void updateDisplay() {

  displayUpdate = false;

  const char U_char[10];
  const char I_char[10];

  dtostrf(current_U, 6, 2, U_char);
  dtostrf(current_I, 6, 2, I_char);

  ssd1306_setFixedFont(ssd1306xled_font8x16);
  //ssd1306_clearScreen();
  ssd1306_printFixed(0,  8, "U in V: ", STYLE_NORMAL);
  ssd1306_printFixed(0, 24, "I in A: ", STYLE_NORMAL);
  ssd1306_printFixed(60,  8, U_char, STYLE_BOLD);
  ssd1306_printFixed(60, 24, I_char, STYLE_BOLD);

}