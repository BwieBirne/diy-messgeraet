
//enums
enum frequency_type { AC,
                      DC,
                      DAC };
enum measurement_type { U,
                        I };

//pins
#define U_PIN A0
#define I_PIN A3

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20

#define U_DIVIDER 40.6f
#define I_DIVIDER 40.0f
#define I_OFFSET 508

#define SQRT2 1.4142

//varibales
enum frequency_type f_type = DC;
enum measurement_type m_type = U;
float current_U = 0.0f;
float current_I = 0.0f;

//timer
#define MEASUREMENT_INTERVAL 1000
long int m_timer = 0;

void setup() {

  Serial.begin(9600);
  pinMode(U_PIN, INPUT);
  pinMode(I_PIN, INPUT);

  Serial.println("A");
}

void loop() {

  timer();
}

void timer() {

  measurement(U_PIN);
}
