
void setup() {

  Serial.begin(9600);
  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);

}

void loop() {

  Serial.println((508-analogRead(A0))/40.0f);
  timer();

}

void timer(){


}

