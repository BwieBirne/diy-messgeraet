
#define BAUD_RATE 115200
#define PIN 2
#define FREQ 1000                 //in Hz
#define MEASUREMENT_INTERVAL 100  //in milliseconds

//timer
uint32_t m_timer = 0;

void setup() {

  Serial.begin(BAUD_RATE);
  pinMode(PIN, OUTPUT);

  Serial.println("\nFrequenzgenerator - 141023.1");

  delay(500);

  Serial.println("Bereit.");
  Serial.println("-------------");
}

void loop() {

  timer();
}

void timer() {

  uint32_t now = millis();

  if (now - m_timer > MEASUREMENT_INTERVAL) {

    m_timer = now;
    digitalWrite(PIN, LOW);
    uint32_t start = micros();
    delayMicroseconds((1e6 / FREQ) / 2);
    digitalWrite(PIN, HIGH);
    delayMicroseconds((1e6 / FREQ) / 2);
    uint32_t stop = micros();

    Serial.print("t: ");
    Serial.print(millis());
    Serial.println(" ms");
    Serial.print("T: ");
    Serial.print((stop - start) / 1e3);
    Serial.println(" ms");
    Serial.print("f: ");
    Serial.print(1e6 / (stop - start));
    Serial.println(" Hz");
    Serial.println("-------------");
  } else {

    digitalWrite(PIN, LOW);
    delayMicroseconds((1e6 / FREQ) / 2);
    digitalWrite(PIN, HIGH);
    delayMicroseconds((1e6 / FREQ) / 2);
  }
}
