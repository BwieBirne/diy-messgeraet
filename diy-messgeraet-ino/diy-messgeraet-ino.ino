
void setup() {

  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);

}

void loop() {

  Serial.println((508-analogRead(A0))/40.0f);
  timer();

}

void timer(){

  measurement(A0);

}

