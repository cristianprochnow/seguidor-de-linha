// Portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6

// Portas sensor QTR
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5

// Valores de ajustes para o seguidor de linha MIF
#define TRESHOLD 600                        // Valor de referencia para cor da linha branca

#define BASE_POWER 100
#define VARIATION_POWER 200 

#define KP 50
#define KI 0 
#define KD 60

int error = 0, previousError = 0, P, I, D, PID;

void setup()
{
//  Serial.begin(9600);
}

void loop()
{
  followLineMEF();
  // readSensors();
}

void readSensors(void) {
  // Função para leitura dos sensores
  Serial.print(analogRead(A0));
  Serial.print(' ');
  Serial.print(analogRead(A1));
  Serial.print(' ');
  Serial.print(analogRead(A2));
  Serial.print(' ');
  Serial.print(analogRead(A3));
  Serial.print(' ');
  Serial.print(analogRead(A4));
  Serial.print(' ');
  Serial.println(analogRead(A5));
  Serial.print(' ');
}

void followLineMEF(void) {
  // Definições das portas digitais
  pinMode(PININ1, OUTPUT);
  pinMode(PININ2, OUTPUT);
  pinMode(PININ3, OUTPUT);
  pinMode(PININ4, OUTPUT);
  pinMode(PINENA, OUTPUT);
  pinMode(PINENB, OUTPUT);
    
    
       if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) <= TRESHOLD) { error = 5; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) <= TRESHOLD
    && analogRead(SENSOR6) <= TRESHOLD) { error = 4; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) <= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = 3; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) <= TRESHOLD
    && analogRead(SENSOR5) <= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = 2; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) <= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = 1; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) <= TRESHOLD
    && analogRead(SENSOR4) <= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = 0; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) <= TRESHOLD
    && analogRead(SENSOR3) <= TRESHOLD
    && analogRead(SENSOR4) <= TRESHOLD
    && analogRead(SENSOR5) <= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = 0; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) <= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = -1; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) <= TRESHOLD
    && analogRead(SENSOR3) <= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = -2; }
  else if (analogRead(SENSOR1) >= TRESHOLD
    && analogRead(SENSOR2) <= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = -3; }
  else if (analogRead(SENSOR1) <= TRESHOLD
    && analogRead(SENSOR2) <= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = -4; }
  else if (analogRead(SENSOR1) <= TRESHOLD
    && analogRead(SENSOR2) >= TRESHOLD
    && analogRead(SENSOR3) >= TRESHOLD
    && analogRead(SENSOR4) >= TRESHOLD
    && analogRead(SENSOR5) >= TRESHOLD
    && analogRead(SENSOR6) >= TRESHOLD) { error = -5; }

    // Calculando PID.
    P = error;
    I = I + error;
    D = error - previousError;
    PID = (KP * P) + (KI * I) + (KD * D);
    previousError = error;

    // Definindo valores de potência do motor.
    int leftMotorSpeed = BASE_POWER - VARIATION_POWER - PID;
    int rightMotorSpeed = BASE_POWER + VARIATION_POWER - PID;

    digitalWrite (PININ1, HIGH);
    digitalWrite (PININ2, LOW);
    digitalWrite (PININ3, LOW);
    digitalWrite (PININ4, HIGH);

    Serial.print(leftMotorSpeed);
    Serial.print(' ');
    Serial.print(rightMotorSpeed);
    Serial.println(' ');

    if (leftMotorSpeed < 0) {
      leftMotorSpeed *= -1;
    }
  
    analogWrite(PINENA, leftMotorSpeed);
    analogWrite(PINENB, rightMotorSpeed);
}
