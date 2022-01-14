//SISTEMA
int estadoSistema;
const int iniciar = 0;
const int espera = 1;
const int finalizar = 2;
boolean haPedido;

//TEMPERATURA
int estadoTemp;
const int ForkTemp = 0;
const int BuscarT = 1;
const int JoinTemp = 2;

byte valorTemp;
boolean tempval;

//ANGULAR_X
int estadoAngX;
const int ForkAngX = 0;
const int BuscarX = 1;
const int JoinAngX = 2;

boolean angvalX;
int valorX;
int phiX;

//ANGULAR_Y
int estadoAngY;
const int ForkAngY = 0;
const int BuscarY = 1;
const int JoinAngY = 2;

boolean angvalY;
int valorY;
int phiY;

//ANGULAR_Z
int estadoAngZ;
const int ForkAngZ = 0;
const int BuscarZ = 1;
const int JoinAngZ = 2;

boolean angvalZ;
int valorZ;
int phiZ;

unsigned long t0, t1x, t1y, t1z;
void Sistema() {

  switch (estadoSistema) {
    case iniciar:
      AtividadeIniciar();
      estadoSistema = espera;
      t0 = millis();

      break;

    case espera :
      if (millis() - t0 >= 100){

        haPedido = true;
        estadoSistema = finalizar;
        
      }

      break;

    case finalizar:
 
      if (!angvalX && !angvalY && !angvalZ && !tempval) {

        escreverDisplay(valorX, valorY, valorZ, valorTemp);
        escreverPython(valorTemp, phiX, phiY, phiZ);
        estadoSistema = iniciar;
        haPedido = false;

      }

      break;

  }

}

void AtividadeIniciar() {

  estadoTemp = ForkTemp;
  estadoAngX = ForkAngX;
  estadoAngY = ForkAngY;
  estadoAngZ = ForkAngZ;
}

void Temp() {

  switch (estadoTemp) {

    case ForkTemp:

      tempval = true;
      estadoTemp = BuscarT;

      break;

    case BuscarT:

      valorTemp = L3GD20_Temperature_Read();
      estadoTemp = JoinTemp;

      break;

    case JoinTemp:
      tempval = false;
      break;

  }
}

void AngX() {

  switch (estadoAngX) {

    case ForkAngX:
      angvalX = true;
      if (haPedido) 
        estadoAngX = BuscarX;
        
      break;

    case BuscarX:
      valorX = L3GD20_X_channel_Read();
      t1x = millis();
      phiX = valorX * (t1x - t0) + phiX;

      estadoAngX = JoinAngX;
      break;

    case JoinAngX:
      angvalX = false;
      break;

  }
}

void AngY() {

  switch (estadoAngY) {

    case ForkAngY:
      angvalY = true;
      if (haPedido) 
        estadoAngY = BuscarY;
        
      break;


    case BuscarY:
      valorY = L3GD20_Y_channel_Read();
      phiY = valorY * (t1y - t0) + phiY;
      estadoAngY = JoinAngY;
      break;

    case JoinAngY:
      angvalY = false;
      break;

  }
}

void AngZ() {

  switch (estadoAngZ) {

    case ForkAngZ:
      angvalZ = true;
      if (haPedido) 
        estadoAngZ = BuscarZ;
      break;

    case BuscarZ:
      valorZ = L3GD20_Z_channel_Read();
      phiZ = valorZ * (t1z + t0) + phiZ;
      estadoAngZ = JoinAngZ;
      break;

    case JoinAngZ:
      angvalZ = false;
      break;

  }
}

void escreverDisplay(int X, int Y, int Z, byte T) {

  displayInit();
  displayClear();
  displaySetCursor(0, 0);
  displayPrintChar(0xF4);
  displayPrintString("x:" + String(X));

  displayPrintChar(0xDF);
  displayPrintChar(0x2F);
  displayPrintChar(0x73);

  displayPrintString(" ");
  displayPrintChar(0xF4);
  displayPrintString("y:" + String(Y));

  displayPrintChar(0xDF);
  displayPrintChar(0x2F);
  displayPrintChar(0x73);

  displaySetCursor(1, 0);
  displayPrintChar(0xF4);
  displayPrintString("z:" + String(Z));

  displayPrintChar(0xDF);
  displayPrintChar(0x2F);
  displayPrintChar(0x73);

  displayPrintString(" T:" + String(T));
  displayPrintChar(0xDF);
  displayPrintChar(0x43);
}

void escreverPython(byte Temp, int PX, int PY, int PZ){
  Serial.println("PhiX: " + String(PX) + "º" );
  Serial.println("PhiY: " + String(PY) + "º" );
  Serial.println("PhiZ: " + String(PZ) + "º" );
  Serial.println("Temp: " + String(Temp) + "º/C" );
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  angvalX = false;
  angvalY = false;
  angvalZ = false;
  tempval = false;
  estadoSistema = iniciar;
  estadoTemp = ForkTemp;
  estadoAngX = ForkAngX;
  estadoAngY = ForkAngY;
  estadoAngZ = ForkAngZ;

  }

  void loop() {
//  displayInit();
//  displayClear();
  Temp();
  AngX();
  AngY();
  AngZ();
  Sistema();


  }
