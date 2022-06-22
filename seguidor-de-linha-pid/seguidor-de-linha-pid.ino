// Portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6

// Portas led rgb
#define PINLEDR 11
#define PINLEDG 9
#define PINLEDB 10

// Portas sensor QTR
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5

// Valores de ajustes para o seguidor de linha MIF
#define TRESHOLD 750                        // Valor de referencia para cor da linha branca
#define SPEED0 240                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 0 0) 
#define SPEED1 200                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 1 0) 

#define SPEED2 180                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 0 0) 
#define SPEED3 150                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 0)  
#define SPEED4 100                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 1) 

#define SPEED5 0                            // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 0) 
#define SPEED6 0                            // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 1) 
#define SPEED7 120                          // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 0 1) 

#define RUNTIME 18500                      // Valor para executar o percurso 

#define KP 50
#define KI 0 
#define KD 50

int error = 0, previousError = 0, P, I, D;

void setup()
{
  Serial.begin(9600);
  rgbControl(255, 0, 0, 1000);
  rgbControl(0, 255, 0, 1000);
  rgbControl(0, 0, 255, 1000);
}

void loop()
{
  followLineMEF();
  //readSensors();
  //motorOption('6',255,255);
}

void motorControl(int speedLeft, int speedRight) {
  // Função para controle do driver de motor

  // Definições das portas digitais
  pinMode(PININ1, OUTPUT);
  pinMode(PININ2, OUTPUT);
  pinMode(PININ3, OUTPUT);
  pinMode(PININ4, OUTPUT);
  pinMode(PINENA, OUTPUT);
  pinMode(PINENB, OUTPUT);

  // Ajustes motor da esquerda
  if (speedLeft < 0) {
    speedLeft = -speedLeft;
    digitalWrite (PININ3, HIGH);
    digitalWrite (PININ4, LOW);
  } else {
    digitalWrite (PININ3, LOW);
    digitalWrite (PININ4, HIGH);
  }

  // Ajustes motor da direita
  if (speedRight < 0) {
    speedRight = -speedRight;
    digitalWrite (PININ1, LOW);
    digitalWrite (PININ2, HIGH);
  } else {
    digitalWrite (PININ1, HIGH);
    digitalWrite (PININ2, LOW);
  }

  analogWrite (PINENA, speedLeft);
  analogWrite (PINENB, speedRight);
}

void motorOption(char option, int speedLeft, int speedRight) {
  // Função para controle de motor com pre definições
  switch (option) {
    case '6': // Esquerda
      motorControl(-speedLeft, speedRight);
      break;
    case '4': // Direita
      motorControl(speedLeft, -speedRight);
      break;
    case '2': // Trás
      motorControl(-speedLeft, -speedRight);
      break;
    case '8': // Frente
      motorControl(speedLeft, speedRight);
      break;
    case '0': // Parar
      motorControl(0, 0);
      break;
  }
}

bool motorStop(long runtime, long currentTime) {
  // Função de parada do robô
  if (millis() >= (runtime + currentTime)) {
    motorOption('0', 0, 0);
    int cont = 0;
    while (true) {
      rgbControl(255, 0, 0, 500);
      rgbControl(0, 0, 0, 500);
      cont++;
    }
    return false;
  }
  return true;
}

void rgbControl(int red, int green, int blue, long rumtime) {
  // Função para controle do led rgb
  pinMode(PINLEDR, OUTPUT);
  pinMode(PINLEDG, OUTPUT);
  pinMode(PINLEDB, OUTPUT);

  digitalWrite(PINLEDR, HIGH);
  digitalWrite(PINLEDG, HIGH);
  digitalWrite(PINLEDB, HIGH);

  analogWrite(PINLEDR, 255 - red);
  analogWrite(PINLEDG, 255 - green);
  analogWrite(PINLEDB, 255 - blue);
  delay(rumtime);
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
  // Função para controle do seguidor de linha em modo de maquina de estado finita
  bool flag = true;
  long currentTime = millis();

  while (flag) {
    // Variáveis nomeadas com o mapeamento da combinação de sensores.
    // Ou seja, se a variável é 000001, então apenas o último sensor (A5)
    // Está com valor.
    bool _000001 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) >= TRESHOLD
    );
    bool _000011 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) >= TRESHOLD
      && analogRead(A5) >= TRESHOLD
    );
    bool _000010 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) >= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _000110 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) >= TRESHOLD
      && analogRead(A3) >= TRESHOLD
      && analogRead(A4) >= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _000100 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) >= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _001100 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) >= TRESHOLD
      && analogRead(A3) >= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _001000 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) >= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _011000 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) >= TRESHOLD
      && analogRead(A2) >= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _010000 = (
      analogRead(A0) <= TRESHOLD
      && analogRead(A1) >= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _110000 = (
      analogRead(A0) >= TRESHOLD
      && analogRead(A1) >= TRESHOLD
      && analogRead(A2) >= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    bool _100000 = (
      analogRead(A0) >= TRESHOLD
      && analogRead(A1) <= TRESHOLD
      && analogRead(A2) <= TRESHOLD
      && analogRead(A3) <= TRESHOLD
      && analogRead(A4) <= TRESHOLD
      && analogRead(A5) <= TRESHOLD
    );
    
         if (_000001) { error = 5; }
    else if (_000011) { error = 4; }
    else if (_000010) { error = 3; }
    else if (_000110) { error = 2; }
    else if (_000100) { error = 1; }
    else if (_001100) { error = 0; }
    else if (_001000) { error = -1; }
    else if (_011000) { error = -2; }
    else if (_010000) { error = -3; }
    else if (_110000) { error = -4; }
    else if (_100000) { error = -5; }
  }
}
