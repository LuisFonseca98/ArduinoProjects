
// SAÍDAS
bool RD, WR, EnA, EnV, EnR, EnF, R1, R0, JMP, JNC, JZ, JOV, PC_0, PC_1;

//Codificacao
bool D9, D8, D7, D6, D5, D4, D3, D2, D1, D0;

//variaveis instrucoes
byte relativo3, relativo2, relativo1, relativo0, end6, Constante8, Constante6;
int relativo5, relativo4;

//entradas e saidas registos/flags
volatile byte  Q_V, Q_R, Q_A, Q_PC, Q_Ov, Q_Cy, Q_Z;
byte D_V, D_R, D_A, D_PC, D_Ov, D_Cy, D_Z, QALU;;

//variaveis clock
int tempoClock = 250;
unsigned long tN_Clock, tB_Clock, tN_MClock, tB_MClock;

word memCodigo[64];

byte memDados[128];

word moduloControlo[16];


void setup() {

  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  Instrucoes();
  attachInterrupt(digitalPinToInterrupt(2), MclkN, RISING);
  interrupts();

}

void loop() {

  MF();
  visualizacoes();

}


void visualizacoes() {

  if (Serial.available()) {
    char recebido = Serial.read();

    switch (recebido) {
      case '1':
        registos();
        break;
      case '2':
        saidas();
        break;
      case '3':
        variaveis();
        break;
      case '4':
        memoriaCodigo();
        break;
      case '5':
        memoriaDados();
        break;
    }
  }
}

void registos() {

  Serial.println();
  Serial.println("Registos");
  Serial.println("");
  Serial.print("V: "); Serial.println(Q_V , HEX);
  Serial.print("R: "); Serial.println(Q_R , HEX);
  Serial.print("A: "); Serial.println(Q_A , HEX);
  Serial.print("PC: "); Serial.println(Q_PC , HEX);
  Serial.println(" ");

}


void saidas() {
  Serial.println();
  Serial.println("Saídas:");
  Serial.println("");

  Serial.print("EnR: " );
  Serial.println(EnR);
  Serial.print("EnV: " );
  Serial.println(EnV);
  Serial.print("EnA: " );
  Serial.println(EnA);
  Serial.print("EnF: " );
  Serial.println(EnF);
  Serial.print("R1: " );
  Serial.println(R1);
  Serial.print("R0: " );
  Serial.println(R0);
  Serial.print("RD: " );
  Serial.println(RD);
  Serial.print("WR: " );
  Serial.println(WR);
  Serial.print("JMP: " );
  Serial.println(JMP);
  Serial.print("JNC: " );
  Serial.println(JNC);
  Serial.print("JZ: " );
  Serial.println(JZ);
  Serial.print("JOV: " );
  Serial.println(JOV);

  Serial.println(" ");
  Serial.println();
}

void variaveis() {
  Serial.println();
  Serial.println("Variaveis:");
  Serial.println("");

  Serial.print("Const8: " ); Serial.println(Constante8 , HEX);
  Serial.print("Const6: " ); Serial.println(Constante6 , HEX);
  Serial.print("Rel5: " ); Serial.println(relativo5 , HEX);
  Serial.print("End6: " ); Serial.println(end6 , HEX);

  Serial.println(" ");
  Serial.println();
}


void memoriaCodigo() {

  Serial.println();
  Serial.println("memoriaCodigo:");
  Serial.println("");

  for (int i = 0; i < 64; i++) {
    Serial.print(i, HEX);
    Serial.print(" : 0x");
    Serial.println(moduloControlo[i], HEX);
  }

  Serial.println(" ");
  Serial.println();
}


void memoriaDados() {
  Serial.println();
  Serial.println("memoriaDados:");
  Serial.println("");

  for (int i = 0; i < 128; i++) {
    Serial.print(i, HEX);
    Serial.print(" : 0x");
    Serial.println(memDados[i], HEX);
  }

  Serial.println(" ");
  Serial.println();
}

void Mclk() {

  tN_Clock = millis();
  if ( (tN_Clock - tB_Clock) > tempoClock) {
    Q_PC = D_PC;

    Serial.print("Instrução: "); Serial.println(memCodigo[Q_PC], HEX);
    Serial.print("Registo PC: "); Serial.println(Q_PC);

    attachInterrupt(digitalPinToInterrupt(2), MclkN, FALLING);
  }

  tB_Clock = tN_Clock;
}


void MclkN() {

  tN_MClock = millis();
  if (tN_MClock - tB_MClock > tempoClock) {

    Q_R   = reg(Q_R, D_R, EnR);
    Q_V   = reg(Q_V, D_V, EnV);
    Q_A   = reg(Q_A, D_A, EnA);
    Q_Z  = reg(Q_Z, D_Z, EnF);
    Q_Cy = reg(Q_Cy, D_Cy, EnF);
    Q_Ov = reg(Q_Ov, D_Ov, EnF);

    Serial.println();
    Serial.print("V = ");
    Serial.println(Q_V, HEX);
    Serial.print("R = ");
    Serial.println(Q_R, HEX);
    Serial.print("A = ");
    Serial.println(Q_A, HEX);
    Serial.print("@R = ");
    Serial.println(memDados[Q_R], HEX);
    Serial.println("FLAGS");
    Serial.print("Z = ");
    Serial.println(Q_Z);
    Serial.print("Cy/Bw = ");
    Serial.println(Q_Cy);
    Serial.print("Ov = ");
    Serial.println(Q_Ov);
    Serial.println();
    Serial.println();
    attachInterrupt(digitalPinToInterrupt(2), Mclk, RISING);
  }
}


byte reg(byte A, byte B, bool Enable) {

  switch (Enable) {
    case 0:
      return A;
    case 1:
      return B;
  }
}

byte somador(byte A, byte B) {

  return A + B;
}

byte ALU(byte A, byte B, boolean Q_Cy, bool S1, bool S0, byte &D_Ov, byte &D_Z, byte &D_Cy) {

  byte AU_AUX;
  int Sum_AUX, Sub_AUX;

  switch (S1 << 1 | S0) {

    case  B00:

      AU_AUX = ~(A | B);

      D_Z = (AU_AUX == 0) ? 1 : 0;

      break;

    case B01:

      Sum_AUX = A + B + Q_Cy;

      D_Cy = ( Sum_AUX > 255) ? 1 : 0;

      AU_AUX = (byte) Sum_AUX & 0xFF;

      D_Z = (AU_AUX == 0) ? 1 : 0;

      D_Ov = ((A < 0 && B < 0 && Sum_AUX > 0) || (A > 0 && B > 0 && Sum_AUX < 0)) ? 1 : 0;

      break;

    case B10:

      Sub_AUX = A - B - Q_Cy;

      D_Cy = (Sub_AUX < 0) ? 1 : 0;

      AU_AUX = (byte) Sub_AUX & 0xFF;

      D_Z = (AU_AUX == 0) ? 1 : 0;

      D_Ov = ((A < 0 && B < 0 && Sub_AUX > 0) || (A > 0 && B > 0 && Sub_AUX < 0)) ? 1 : 0;

      break;

  }
  return AU_AUX;

}

void MF() {

  byte AUX1, AUX0;

  int memCodigoI = 0;

  memCodigoI |= ((bitRead(memCodigo[Q_PC], 9) << 3) | (bitRead(memCodigo[Q_PC], 8) << 2) | (bitRead(memCodigo[Q_PC], 7) << 1) |
                 (bitRead(memCodigo[Q_PC], 6) << 0));

  int rel5F = B00011111 & memCodigo[Q_PC];

  relativo4 = (bitRead(rel5F, 4) * - 16);
  relativo3 = (bitRead(rel5F, 3) * 8);
  relativo2 = (bitRead(rel5F, 2) * 4);
  relativo1 = (bitRead(rel5F, 1) * 2);
  relativo0 = (bitRead(rel5F, 0) * 1);

  D9 = bitRead(memCodigo[Q_PC],  9);
  D8 = bitRead(memCodigo[Q_PC],  8);
  D7 = bitRead(memCodigo[Q_PC],  7);
  D6 = bitRead(memCodigo[Q_PC],  6);
  D5 = bitRead(memCodigo[Q_PC],  5);
  D4 = bitRead(memCodigo[Q_PC],  4);
  D3 = bitRead(memCodigo[Q_PC],  3);
  D2 = bitRead(memCodigo[Q_PC],  2);
  D1 = bitRead(memCodigo[Q_PC],  1);
  D0 = bitRead(memCodigo[Q_PC],  0);

  JOV  = bitRead(moduloControlo[memCodigoI],   11);
  JZ   = bitRead(moduloControlo[memCodigoI],   10);
  JNC  = bitRead(moduloControlo[memCodigoI],   9);
  JMP  = bitRead(moduloControlo[memCodigoI],   8);
  R1   = bitRead(moduloControlo[memCodigoI],   7);
  R0   = bitRead(moduloControlo[memCodigoI],   6);
  EnR  = bitRead(moduloControlo[memCodigoI],   5);
  EnA  = bitRead(moduloControlo[memCodigoI],   4);
  EnV  = bitRead(moduloControlo[memCodigoI],   3);
  RD   = bitRead(moduloControlo[memCodigoI],   2);
  WR   = bitRead(moduloControlo[memCodigoI],   1);
  EnF  = bitRead(moduloControlo[memCodigoI],   0);

  PC_0 = ( (JNC & !Q_Cy) | (JZ & Q_Z) | (JOV & Q_Ov));
  PC_1 = JMP;

  relativo5 = relativo4 + relativo3 + relativo2 + relativo1 + relativo0;

  end6 = B00111111 & memCodigo[Q_PC];

  AUX0 = mux_2x1(1, relativo5, PC_0);

  AUX1 = somador(AUX0, Q_PC);

  D_PC = mux_2x1(AUX1, end6, JMP);

  D_R = end6;

  Constante8 = B11111111 & memCodigo[Q_PC];

  Constante6 = B00111111 & memCodigo[Q_PC];

  QALU = ALU(Q_V, Q_A, Q_Cy, D7, D6, D_Ov, D_Z, D_Cy);

  D_V = mux_4x1(0, Constante8, memDados[Q_R], QALU, R1, R0);

  D_A = Q_V;

  memDados[Q_R] = !WR ? Q_V : memDados[Q_R];

}


void Instrucoes() {

  memCodigo[0X00] = 0xFE; //MOV V, CONST8
  memCodigo[0X01] = 0x108; //MOV R, CONST6
  memCodigo[0X02] = 0x303; // JNC REL5
  memCodigo[0X02] = 0x2B1; // JZ REL6
  memCodigo[0X02] = 0x2DA; // Jov REL6
  memCodigo[0X02] = 0x3E9; // JMP REL6
  memCodigo[0X02] = 0x140; //MOV A, V
  memCodigo[0X03] = 0x180; //MOV V, @R
  memCodigo[0X04] = 0xFE; //MOV V, CONST8
  memCodigo[0X05] = 0x1C0; //MOV @R, V
  memCodigo[0X06] = 0x240; // ADD V, A
  memCodigo[0X06] = 0x280; // SUBB V, A
  memCodigo[0X07] = 0x140; //MOV A, V
  memCodigo[0X08] = 0x280; //NOR V, A
  
//  memCodigo[0X00] = 0x021; //MOV V, CONST8//
//  memCodigo[0X01] = 0x109; //MOV R, CONST6
//  memCodigo[0X02] = 0x303; // JNC REL5
//  memCodigo[0X02] = 0x2B1; // JZ REL6
//  memCodigo[0X02] = 0x2DA; // Jov REL6
//  memCodigo[0X02] = 0x3E9; // JMP REL6
//  memCodigo[0X01] = 0x140; //MOV A, V
//  memCodigo[0X02] = 0x020; //MOV V, CONST8
//  memCodigo[0X03] = 0x180; //MOV V, @R
//  memCodigo[0X03] = 0x240; // ADD V, A

}

byte mux_2x1(byte A, byte B, bool S) {

  switch (S) {
    case 0:
      return A;
    case 1:
      return B;
  }
}


byte mux_4x1(byte A, byte B, byte C, byte D, bool S1, bool S0) {

  switch (S1 << 1 | S0) {
    case B11:
      return A;
    case B10:
      return B;
    case B01:
      return C;
    case B00:
      return D;
  }
}

void fillEPROM() {

  int i;

  moduloControlo[0x04] = 0x840;
  moduloControlo[0x05] = 0x680;
  moduloControlo[0x06] = 0x320;
  moduloControlo[0x07] = 0x400;
  moduloControlo[0x08] = 0x720;
  moduloControlo[0x09] = 0xF20;
  moduloControlo[0x10] = 0xD20;
  moduloControlo[0x11] = 0x604;
  moduloControlo[0x12] = 0x602;
  moduloControlo[0x13] = 0x601;
  moduloControlo[0x14] = 0x608;

  for (i = 0x00; i <= 0x03; i++) {
    moduloControlo[i] = 0x730;
  }
}
