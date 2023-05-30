
//enums
enum freq_type {AC, DC, DAC};
enum m_type {U, I};

//pins
#define U_PIN A0
#define I_PIN A3

//constants
#define MEASUREMENT_ITR 100
#define MEASUREMENT_DELAY 20

//varibales
enum freq_type = DC;
enum m_type = U;
float current_U = 0.0f;
float current_I = 0.0f;

//timer
#define MEASUREMENT_INTERVAL 1000
long int m_timer = 0;