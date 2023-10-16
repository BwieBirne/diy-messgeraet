
#define BAUD_RATE 115200
#define PIN A0
#define VOLTAGE_DIV 204.6f

void setup() {

  Serial.begin(BAUD_RATE);
  pinMode(PIN, INPUT);

}

void loop() {
  
  delayMicroseconds(20);
  Serial.println(analogRead(PIN) / VOLTAGE_DIV);
}
