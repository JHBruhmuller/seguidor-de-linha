// portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6
 
// portas do sensor QTR
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4
#define S6 A5
 
// não deixar o dengue por tudo no máximo. 

// nível de ajuste 
#define TRESHOLD 700 
// a cor da linha 
#define SPEED0 255 
// Valor pra
//o sensor (0 0 1 1 0 0)
#define SPEED1 255
 // Valor pro sensor (0 0 1 1 1 0)
#define SPEED2 255
 // sensor (0 0 0 1 0 0)
#define SPEED3 255 
//sensor (0 0 0 1 1 0)
#define SPEED4 255
 //  sensor (0 0 0 1 1 1)
#define SPEED5 255
 // sensor (0 0 0 0 1 0)
#define SPEED6 0
 // sensor (0 0 0 0 1 1)
#define SPEED7 255 
// sensor (0 0 0 0 0 1)
#define RUNTIME 20000 
//  TEMPO **************
//Dados do PID são definidos
int Kp = 0.5;//0.5
int Ki = 0.2;//
int Kd = 0.8;//0.5
int I = 0, P = 0, D = 0, PID = 0;
int erro, erro_anterior =0;
float calcula_PID();
void setup() {
Serial.begin(9600);
}
 
void loop() {
// TESTE 1°: leituta sensor
//int s[6];
//readSensors(true, s);
// TESTE 2°: motor esquerda
// motorOption('4',255,255);
//TESTE 3°: motor direita
// motorOption('6', 255, 255);
// TESTE 4°: seguidor de linha
//followLineMEF();
// TESTE 5°: teste led RGB
// rgbControl(255,0,0,0);
followLineMEF();
calcula_PID();
}
 
void motorControl(int speedLeft, int speedRight) {
// 
// definindo as portas
pinMode(PININ1, OUTPUT);
pinMode(PININ2, OUTPUT);
pinMode(PININ3, OUTPUT);
pinMode(PININ4, OUTPUT);
pinMode(PINENA, OUTPUT);
pinMode(PINENB, OUTPUT);
// Ajst squerda
if (speedLeft <= 0) {
speedLeft = -speedLeft;
digitalWrite (PININ3, HIGH);
digitalWrite (PININ4, LOW);
} else {
digitalWrite (PININ3, LOW);
digitalWrite (PININ4, HIGH);
}
// Ajst direita
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
switch (option) {
case '8': // Frente
motorControl(-speedLeft, speedRight);
break;
case '2': // Tras
motorControl(speedLeft, -speedRight);
break;
case '4': // Esqueda
motorControl(-speedLeft-calcula_PID(), -speedRight-calcula_PID());
break;
case '6': // Direita
motorControl(speedLeft+calcula_PID(), speedRight+calcula_PID());
break;
case '0': // Parar
motorControl(0, 0);
break;
}
}
 
bool motorStop(long runtime, long currentTime) {
// Função pra Parar 
if (millis() >= (runtime + currentTime)) {
motorOption('0', 0, 0);
int cont = 0;
while (true){
cont++;
}
return false;
}
return true;
}
 
void readSensors(bool readSerial, int *sensors) {
// leitura dos sensores
sensors[0] = analogRead(S1);
sensors[1] = analogRead(S2);
sensors[2] = analogRead(S3);
sensors[3] = analogRead(S4);
sensors[4] = analogRead(S5);
sensors[5] = analogRead(S6);
if (readSerial) {
Serial.print(sensors[0]);
Serial.print(' ');
Serial.print(sensors[1]);
Serial.print(' ');
Serial.print(sensors[2]);
Serial.print(' ');
Serial.print(sensors[3]);
Serial.print(' ');
Serial.print(sensors[4]);
Serial.print(' ');
Serial.println(sensors[5]);
}
}
 
void followLineMEF(void) {

bool flag = true;
long currentTime = millis();
while (flag) {
// Flag  checar parada
flag = motorStop(RUNTIME, currentTime);
// lida dos sensores
int s[6];
readSensors(false, s);
// leitura do sensor (1 1 1 1 1 1)
if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
motorOption('8', SPEED0, SPEED0);
erro=0;
// sensor (0 1 1 1 1 0)
} else if ( s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED0);
erro=0;
// sensor (0 0 1 1 0 0)
} else if ( s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED0);
erro=0;
// sensor (0 1 1 1 0 0)
} else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED1);
erro=1;
//  sensor (0 0 1 1 1 0)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD ) {
motorOption('8', SPEED1, SPEED0);
erro=1;
// sensor (0 0 1 0 0 0)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED2);
erro=1;
// ssensor (0 0 0 1 0 0)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD ) {
motorOption('8', SPEED2, SPEED0);
erro=1;
// sensor (0 1 1 0 0 0)
} else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED3);
erro=2;
// sensor (0 0 0 1 1 0)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED3, SPEED0);
erro=2;
// sensor (1 1 1 0 0 0)
} else if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED4);
erro=3;
// sensor (0 0 0 1 1 1)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
motorOption('8', SPEED4, SPEED0);
erro=3;
// sensor (0 1 0 0 0 0)
} else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED5);
erro=4;
// sensor (0 0 0 0 1 0)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED5, SPEED0);
erro=4;
//sensor (1 1 0 0 0 0)
} else if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('8', SPEED0, SPEED6);
erro=5;
// sensor (0 0 0 0 1 1)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
motorOption('8', SPEED6, SPEED0);
erro=5;
//  sensor (1 0 0 0 0 0)
} else if (s[0] <= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
motorOption('6', SPEED7, SPEED7);
erro=6;
// semsor (0 0 0 0 0 1)
} else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] <= TRESHOLD) {
motorOption('4', SPEED7, SPEED7);
erro=6;
}
}
}
float calcula_PID() {
if (erro != 0) {
P = erro;
I += erro;
if (I > 255) I = 255;
else if (I < -255) I = -255;
D = erro - erro_anterior;
PID = (Kp * P) + (Ki * I) + (Kd * D);
erro_anterior = erro;
return PID;
}
return 0;
