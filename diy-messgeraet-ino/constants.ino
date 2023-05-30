
//enums
enum frequency_type {AC, DC, DAC};
enum measurement_type {U, I};

//pins
#define U_PIN A0
#define I_PIN A3

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20

//varibales
enum frequency_type f_type  = DC;
enum measurement_type m_type = U;
float current_U = 0.0f;
float current_I = 0.0f;

//timer
#define MEASUREMENT_INTERVAL 1000
long int m_timer = 0;