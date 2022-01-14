#define RS 0x01
#define RW 0x02
#define EN 0x04
#define LUZ 0x08
#define ENDERECO 0x3F
#include <Wire.h>


void escreverDados4(byte quatrobits) {
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ| RS);
  Wire.endTransmission();
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | RS | EN );
  Wire.endTransmission();
  delayMicroseconds(1); // enable ativo >450ns
  Wire.beginTransmission(ENDERECO);
  Wire.write((quatrobits << 4) | LUZ | RS);
  Wire.endTransmission();
  delayMicroseconds(40); // tempo > 37us para comando fazer efeito
}

void escreverComandos4(byte quatrobits){
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

void iniciarDisplay(){
  //Init display
  delay(50);
  escreverComandos4(0x3);
  delay(5);
  escreverComandos4(0x3);
  delayMicroseconds(200);
  escreverComandos4(0x3);
  escreverComandos4(0x2);
  //Function set
  escreverComandos4(0x2);
  escreverComandos4(0x8);
  delayMicroseconds(120);
  //Blink ON curson ON display ON
  escreverComandos4(0x0);
  escreverComandos4(0xF);
  delayMicroseconds(120);
  //Set cursor
  escreverComandos4(0x8);
  escreverComandos4(0x0);
  delayMicroseconds(120);
}

void setcursor(int linha, int coluna){
  if( 0 <= linha <= 1 and  0 <= coluna <= 15){
    byte MSB = (byte)((linha * 4) | 0x8);
    byte LSB = (byte) coluna;
    escreverComandos4(MSB);
    escreverComandos4(LSB);
    delayMicroseconds(120);
  }
}

void printChar(char c){
  escreverDados4(c >> 4); // 4 primeiros
  escreverDados4(c);  //4 ultimos
  delayMicroseconds(120);
}

void printString(String s){
   for(int i=0; i < s.length(); i++){ //Print de uma string com o printChar
      printChar(s[i]); 
   }
}

void clearDisplay(){
  escreverComandos4(0x0);
  escreverComandos4(0x1);
}

void limpaLinha(){
  printString("                ");
}

