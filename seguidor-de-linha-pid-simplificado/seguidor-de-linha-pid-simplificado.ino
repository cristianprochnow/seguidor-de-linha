#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5

#define PIND_1 2 // esquerda
#define PIND_2 4 // esquerda
#define PIND_3 5 // direita
#define PIND_4 7 // direita

#define PINA_A 3 // esquerda ANALÓGICO
#define PINA_B 6 // direita ANALÓGICO

int sensors[6];
int speed_A = 200, speed_B = 255;
int speed_left = 0, speed_right = 0;
int KI = 0, KP = 35, KD = 35;
int I = 0, P = 0, D = 0, PID = 0;
int error = 0, previous_error = 0;

void setup()
{
  pinMode(PIND_1, OUTPUT);
  pinMode(PIND_2, OUTPUT);
  pinMode(PIND_3, OUTPUT);
  pinMode(PIND_4, OUTPUT);

  pinMode(PINA_A, OUTPUT);
  pinMode(PINA_B, OUTPUT);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);
}

void loop()
{
  
}
