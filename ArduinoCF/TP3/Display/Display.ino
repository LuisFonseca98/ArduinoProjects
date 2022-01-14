#define RS 0x01
#define RW 0x02
#define EN 0x04
#define LUZ 0x08
#define ENDERECO 0x27
#include <Wire.h>

void escreverDados4(byte quatrobits) {
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | RS);
  Wire.endTransmission();
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | RS | EN );
  Wire.endTransmission();
  delayMicroseconds(1);
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | RS);
  Wire.endTransmission();
  delayMicroseconds(40);
}

void escreverComandos4(byte quatrobits) {
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ);
  Wire.endTransmission();
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | EN );
  Wire.endTransmission();
  delayMicroseconds(1); // enable ativo >450ns
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ );
  Wire.endTransmission();
  delayMicroseconds(40); // tempo > 37us para comando fazer efeito
}


void escreverDados8(byte oitoBits) {
  escreverDados4(oitoBits >> 4);
  escreverDados4(oitoBits & 0xF);
}

void escreverComandos8(byte oitoBits) {
  escreverComandos4(oitoBits >> 4);
  escreverComandos4(oitoBits & 0xF);
}

void displayInit() {
  Wire.begin();
  delay(50);
  escreverComandos4(0x03);
  delay(5);
  escreverComandos4(0x03);
  delayMicroseconds(200);

  escreverComandos8(0x32);

  //Funcao set
  escreverComandos4(0x02);
  escreverComandos4(0x08);

  delayMicroseconds(120);

  displayCursorOffBlinckOff();

  displayCursorOnBlinckOn();

  //set cursor
  escreverComandos4(0x08);
  escreverComandos4(0x00);

  delayMicroseconds(120);

}

void displayClear() {
  escreverComandos8(0x01);
  delay(5);
}

void displaySetCursor(byte linha, byte coluna) {
  if ( 0 <= linha <= 2 and  0 <= coluna <= 16) {
    byte MSB = (byte)((linha * 4) | 0x8);
    byte LSB = (byte) coluna;
    escreverComandos4(MSB);
    escreverComandos4(LSB);
    delayMicroseconds(120);
  }
}


void displayCursorOffBlinckOff() {
  escreverComandos8(0x08);
  delayMicroseconds(120);
}

void displayCursorOnBlinckOn() {
  escreverComandos8(0x0F);
  delayMicroseconds(120);
}

void displayPrintChar(char c) {
  escreverDados8(c);
  delayMicroseconds(120);
}


void displayPrintString(String s) {
  for (int i = 0; i < s.length(); i++) {
    displayPrintChar(s[i]);
  }
}

//void setup() {Serial.begin(9600);}
//
//void loop() {
//  displayInit();
//  displayClear();
//  displaySetCursor(0, 0);
//  displayPrintString("Hello World");
//  displaySetCursor(1, 0);
//  displayPrintString("CF Project");
//  delay(2000);
//}
