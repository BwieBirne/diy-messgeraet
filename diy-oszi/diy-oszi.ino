
#define BAUD_RATE 115200
#define PIN A0
#define INPUT_VOLTAGE 3.3f //in V
#define ADC_RESOLUTION 1023
#define VOLTAGE_DIV (float)(ADC_RESOLUTION / INPUT_VOLTAGE)

void setup() {

  Serial.begin(BAUD_RATE);
  pinMode(PIN, INPUT);
  while (!Serial);

}

void loop() {
  
  delayMicroseconds(20);
  Serial.println(analogRead(PIN) / VOLTAGE_DIV);
}
