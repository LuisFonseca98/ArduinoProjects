//incluir biblioteca do servo
#include <Servo.h>

//servo para controlar a direcao
Servo servoControlaDirecao;

//pinos para os piscas de frente
#define LED_AMARELO_1 2 //pisca frente
#define LED_AMARELO_2 10 //pisca frente
#define LED_BRANCO_1 4 // pisca frente
#define LED_BRANCO_2 5 //pisca frente

//pinos para os piscas de tras
#define LED_AMARELO_3 6 //piscas atras
#define LED_AMARELO_4 7 //piscas atras
#define LED_VERMELHO_1 8 //piscas atras
#define LED_VERMELHO_2 9 //piscas atras

//Pinos para o butao e potenciometro e buzzer
#define BOTAO A2
#define BUZZER 11
#define MDC A4

//leds para a cor RGB
#define red 1            
#define green 12        
#define blue 13         

//Variaveis globais
int estadoBotao = 0;
int valor = 0;
int bright,color;
String ler = "";

void setup() {
  Serial.begin(9600);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  servoControlaDirecao.attach(3); // le o pino do servo
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A2, INPUT);


}
void loop() {
  condutorAbreCarro();
  automatoServo();
  automatoLDR();
  ledBranco ();
  corLed ();
  automatoMdc();
}



void condutorAbreCarro() {
  if (buttonRead() == true) {
    Serial.println("o condutor abriu a porta");
    digitalWrite(2, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    buzzer();

  } else {
    digitalWrite(2, LOW);
    digitalWrite(10, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
}

void automatoServo() {
  int pos = 0;
  switch (Serial.read()) {
    case 'e':
      for (pos = 90; pos < 180; pos += 1) {
        servoControlaDirecao.write(pos);
        Serial.println("Esquerda");
        digitalWrite(2, HIGH);
        digitalWrite(6, HIGH);
      }
      break;
    case 'd':
      for (pos = 90; pos > 0; pos -= 1) {
        servoControlaDirecao.write(pos);
        Serial.println("Direita");
        digitalWrite(10, HIGH);
        digitalWrite(7, HIGH);
      }
      break;
    case 'f':
      servoControlaDirecao.write(90);
      Serial.println("Frente");
      break;
  }
}
void automatoLDR() {
  int estado;
  int valorLDR = analogRead(A0);
  int valor25 = 255;
  int valor50 = 512;
  int valor1023 = 1023;
  unsigned long int t, timeStamp = millis();
  int intervalo = t - timeStamp;
  switch (estado) {
    case 0:
      while (valorLDR < 255) {
        analogWrite(10, valor25);
        analogWrite(4, valor25);
        if (intervalo > 2000) {
          estado = 1;
        }
      }
      break;
    case 1:
      while (valorLDR > 255 && valorLDR < 513) {
        analogWrite(10, valor50);
        analogWrite(4, valor50);
        if (intervalo > 3000) {
          estado = 2;
        }
      }
      break;
    case 2:
      while (valorLDR > 513 && valorLDR < 1024) {
        analogWrite(10, valor1023);
        analogWrite(5, valor1023);
        if (intervalo > 4000) {
          estado = 0;
        }
      }
      break;
  }
}

void buzzer () {
  tone( 11, 250, 500);
  delay(250);
  tone( 11, 500, 500);
  delay(250);
  tone( 11, 1000, 500);
  delay(250);
  tone( 11, 1500, 500);
  delay(250);
}

void ledBranco () {
  if (Serial.available() >= 0) {
    bright = Serial.read();

    switch (bright) {
      case '0':
        analogWrite (8, 0);
        analogWrite (9, 0);
        Serial.println("Desligado");
        break;
      case '1':
        analogWrite(8, 85);
        analogWrite (9, 85);
        Serial.println("Minimos");
        break;
      case '2':
        analogWrite(8, 170);
        analogWrite (9, 170);
        Serial.println("Medios");
        break;
      case '3':
        analogWrite(8, 255);
        analogWrite(9, 255);
        Serial.println("Maximos");
        break;
    }
  }
}


void corLed() {
  switch (color) {
    case 0:
      if ( ler = "red") {
        analogWrite(red, 255);   //  Atribui o valor ao Led
        analogWrite(green, 0);
        analogWrite(blue, 0);
        break;
      }
    case 1:
      if (ler = "green") {
        analogWrite(red, 0);   //  Atribui o valor ao Led
        analogWrite(green, 255);
        analogWrite(blue, 0);
        break;
      }
    case 2:
      if (ler = "blue") {
        analogWrite(red, 0);   // Atribui o valor ao Led
        analogWrite(green, 0);
        analogWrite(blue, 255);
        break;
      }
    case 3:
      if (ler = "purple") {
        analogWrite(red, 255);   // Atribui o valor ao Led
        analogWrite(green, 0);
        analogWrite(blue, 255);
        break;
      }
    case 4:
      if (ler = "brown") {
        analogWrite(red, 255);   // Atribui o valor ao Led
        analogWrite(green, 255);
        analogWrite(blue, 0);
        break;
      }
    case 5:
      if (ler = "yellow") {
        analogWrite(red, 255);   // Atribui o valor ao Led
        analogWrite(green, 0);
        analogWrite(blue, 255);
        break;
      }
    case 6:
      if (ler = "white") {
        analogWrite(red, 255);   // Atribui o valor ao Led
        analogWrite(green, 255);
        analogWrite(blue, 255);
        break;
      }
    case 7:
      if (ler = "no color") {
        analogWrite(red, 0);   // Atribui o valor ao Led
        analogWrite(green, 0);
        analogWrite(blue, 0);
        break;
      }
    default:
      if (ler = "") {
        Serial.println("Escolha entre: red, green, blue, purple, brown, yellow, white, no color.");
      }
  }
}


void automatoMdc(){
  static const int PIN = MDC;
  int f = 1000;
  int duty = 25;
  unsigned long T = 1000000/f;
  unsigned long TH = T * duty / 100;
  unsigned long TL = T - TH;
  static const int NOPULSE = 0, PULSE = 1;
  static int state = NOPULSE ;
  static unsigned long t,timeStamp = micros();
  switch(state){
    case NOPULSE:
    t = micros();
    if((t-timeStamp)> (TL)){
      digitalWrite(PIN,HIGH);
      timeStamp = micros();
      state = PULSE;
    }
    break;
    case PULSE:
    t = micros();
    if((t-timeStamp ) > (TH)){
      digitalWrite(PIN,LOW);
      timeStamp = micros();
      state = NOPULSE;
    }
    break;
    }
  }


  
////FUNCOES AUXILIARES/////
boolean buttonRead() {
  estadoBotao = digitalRead(A2);
  if (estadoBotao == HIGH) {
    return true;
  } else {
    return false;
  }
  return true;
}

bool caracterSelected(char key) {
  if (Serial.available() >= 0) {
    char letra = Serial.read();
    if (letra == key) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}
