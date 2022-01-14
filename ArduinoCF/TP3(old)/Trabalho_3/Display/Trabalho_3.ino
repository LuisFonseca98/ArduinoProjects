#include <Wire.h>

#define BMP085_ADDRESS 0x77  // I2C adress BMP180

int OSS = 0;  // Oversampling Setting

int AC1;
int AC2;
int AC3;
unsigned int AC4;
unsigned int AC5;
unsigned int AC6;
int b1;int B2;int MB;int MC;int MD; 
long UT;long UP;long X1, X2,X3;long T;
long B3,B5,B6;
unsigned long B4,B7;
long p;
const float p0 = 101325;     // Pressao ao nivel do mar
float altitude;


void setup()
{
  Serial.begin(9600);
  Wire.begin();  
}

void loop(){
  LerCalibracao();
  LerUT();
  LerUP();
  CalcularTemperatura();
  CalcularPressao();
  
  altitude = (float)44330 * (1 - pow(((float) p/p0), 0.190295)); 
  
  Serial.print("Temperature: ");Serial.print(T*0.1);Serial.println(" ºC");
  Serial.print("Pressure: ");Serial.print(p);Serial.println(" Pa");
  Serial.print("Altitude: ");Serial.print(altitude);Serial.println(" m");
  Serial.println();
  iniciarDisplay();
  setcursor(0,0);
  limpaLinha();
  setcursor(1,0);
  limpaLinha();
  setcursor(0,0);
  printString("Temp:" + (String) (T*0.1) + " C");
  setcursor(1,0);
  printString("Pres:" + (String) p + " Pa");
  delay(2000);
  setcursor(0,0);
  limpaLinha();
  setcursor(1,0);
  limpaLinha();
  setcursor(0,0);
  printString("Alt:" + (String) (altitude) + " m");
  delay(2000);
}


void LerCalibracao(){
  AC1 = LerRegisto16Bits(0xAA);
  AC2 = LerRegisto16Bits(0xAC);
  AC3 = LerRegisto16Bits(0xAE);
  AC4 = LerRegisto16Bits(0xB0);
  AC5 = LerRegisto16Bits(0xB2);
  AC6 = LerRegisto16Bits(0xB4);
  b1 = LerRegisto16Bits(0xB6);
  B2 = LerRegisto16Bits(0xB8);
  MB = LerRegisto16Bits(0xBA);
  MC = LerRegisto16Bits(0xBC);
  MD = LerRegisto16Bits(0xBE);
}


byte LerRegisto8Bits(unsigned char address){
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(Wire.available() < 0 );
    
  return Wire.read();
}


int LerRegisto16Bits(unsigned char address){
  byte MSB, LSB;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2);
  MSB = Wire.read();
  LSB = Wire.read();
  
  return (int) MSB<<8 | LSB;
}

void EscreverRegisto16Bits(int address, int value){
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}


void LerUT(){
  // Escrever 0x2E no registo 0xF4
  EscreverRegisto16Bits(0xF4,0x2E);
  // Esperar 4.5s pelo menos
  delay(5);
  UT = LerRegisto16Bits(0xF6);
}

// Read the uncompensated pressure value
void LerUP(){
  byte MSB, LSB,XLSB;
  EscreverRegisto16Bits(0xF4,0x34 + (OSS<<6));
  
  delay(2 + (3<<OSS));
  
  // Ler registos 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
 
  MSB = LerRegisto8Bits(0xF6);
  LSB = LerRegisto8Bits(0xF7);
  XLSB = LerRegisto8Bits(0xF8);
  UP = (((unsigned long) MSB << 16) | ((unsigned long) LSB << 8) | (unsigned long) XLSB) >> (8-OSS);
}


long CalcularTemperatura(){
  X1 =((UT - AC6)*AC5)>>15;
  X2 = ((long)MC<<11)/(X1 + MD);
  B5 = X1 + X2;
  T = (B5 + 8)>>4;  
}


long CalcularPressao(){
   B6 = B5 - 4000;

   X1 =(B2 *((long)(B6*B6>>12)))>>11;

   X2 = (AC2*B6)>>11;

   X3 = X1+X2;
 
   B3 = (((AC1*4+X3)<<OSS)+2)/4;
   
   X1 = (AC3*B6)>>13;
   
   X2 = ((long)b1 * ((long)(B6*B6)>>12))>>16;
   
   X3 =((X1+X2)+2)>>2;

   B4 = AC4*(unsigned long)(X3+32768)>>15;

   B7 = ((unsigned long)UP-B3)*(50000>>OSS);

   if (B7 < 0X8000000) {
    p = (B7*2)/B4;
    }
   else {
    p = (B7/B4)*2;
   }
   X1 = (p>>8)*(p>>8);

   X1 = (X1 * 3038)>>16;

   X2 = (-7357 * p)>>16;

   p = p + ((X1 + X2 + 3791)>>4);

}
