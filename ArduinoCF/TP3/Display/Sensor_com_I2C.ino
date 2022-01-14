#include <Wire.h>

#define L3GD20_ENDERECO 0x69

#define OUT_X_LOW 0x28
#define OUT_X_HIGH 0x29

#define OUT_Y_LOW 0x2A
#define OUT_Y_HIGH 0x2B

#define OUT_Z_LOW 0x2C
#define OUT_Z_HIGH 0x2D

#define OUT_TEMP 0x26

#define sensorInit 0x20

#define WHO_AM_I 0x0F

byte L3GD20_read_8bit_value(byte regAddress) {

  Wire.beginTransmission(L3GD20_ENDERECO);
  Wire.write(regAddress);
  Wire.endTransmission();

  Wire.requestFrom(L3GD20_ENDERECO, 1);
  while (Wire.available() < 0)
    return Wire.read();

}


void L3GD20_write_8bit_value(byte regAddress, byte value) {

  Wire.beginTransmission(L3GD20_ENDERECO);
  Wire.write(regAddress);
  Wire.write(value);
  Wire.endTransmission();

}

bool L3GD20_Init() {


  L3GD20_write_8bit_value(sensorInit, 0x0F);

  if (L3GD20_read_8bit_value(0x0F) == WHO_AM_I) {
    return true;
  } else {
    return false;
  }
}


int L3GD20_X_channel_Read() {

  byte xLow = L3GD20_read_8bit_value(OUT_X_LOW);

  byte xHigh = L3GD20_read_8bit_value(OUT_X_HIGH);

  float fs = fatorSensitividade(250);

  int velX = (int) (xHigh << 8 | xLow) * fs;

  return velX;

}


int L3GD20_Y_channel_Read() {
  byte yLow = L3GD20_read_8bit_value(OUT_Y_LOW);
  byte yHigh = L3GD20_read_8bit_value(OUT_Y_HIGH);

  float fs = fatorSensitividade(250);

  int velY = (int) (yHigh << 8 | yLow) * fs;

  return velY;
}


int L3GD20_Z_channel_Read() {

  byte zLow = L3GD20_read_8bit_value(OUT_Z_LOW);

  byte zHigh = L3GD20_read_8bit_value(OUT_Z_HIGH);

  float fs = fatorSensitividade(250);

  int velZ = (int) (zHigh << 8 | zLow) * fs;

  return velZ;
}



byte L3GD20_Temperature_Read() {
  return 28 - L3GD20_read_8bit_value(OUT_TEMP);
}

float fatorSensitividade(int v) {
  float valor = 0.0;

  if (v = 250) {
    valor = 8.75 * pow(10, -3);
  } else if (v == 500) {
    valor = 17.50 * pow(10, -3);
  } else if (v == 2000) {
    valor = 70 * pow(10, -3);
  }
  return valor;
}
