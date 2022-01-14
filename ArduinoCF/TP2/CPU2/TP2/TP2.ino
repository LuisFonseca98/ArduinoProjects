boolean imprimir;
int adress;
//SAIDAS MODULO CONTROLO
boolean ENV, ENA, ENR, ENZ, ENOV, ENCB;
boolean WR, RD, SPC0, SPC1, SA0, SA1, JNC, JOV, JMP;

boolean AD3, AD2, AD1, AD0; //enderecos da eprom
int memoriaCodigo[64];
int memoriaDados[64] = {0, 0, 0};
int rel5, end6;
int const8, const6;
int DBC, ABC;
int DBD, ABD;
int DPC, DCY, DV, DZ, DR, DOV, QPC, QV, QCYB, QZ, QOV, DA, QA, QR; //entradas e saidas dos registos
int DSum, AUY, AUR;
boolean PCclock;
unsigned long tempoClock;
const unsigned long T_filtro = 250;
const int EPROM_MC[32] = {0x0, 0x0,
                          0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
                          0x2, 0x2,
                          0x3,
                          0x4,
                          0x5,
                          0x6,
                          0x7,
                          0x8,
                          0x9,
                          0xA, 0xC, 0xD, 0xE, 0xF
                         };

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  PCclock = digitalRead(2); //ler o pino do botao
  tempoClock = millis();
  attachInterrupt(0, Clock, RISING);
  imprimir = true;
  reset();
  carregarPrograma();
  interrupts();
}

void Clock() {
  """Funcao que faz o clock quando pressionado por acao de um botao, com efeito ascendente"""
  if (PCclock == HIGH) {
    if (millis() - tempoClock >= T_filtro) {
      tempoClock = millis();
      //reset();
      MFSequencial();
      imprimir = true;
    }
  } else {
    PCclock = digitalRead(2);
  }
}

void loop() {
  MCComb();
  MFComb();
  if (imprimir) {
    imprimirRegistos();
    MCComb();
    MFComb();
    imprimir = false;
  }
}

void reset() {
  """Da clear em todos os registos, colocando todas as entradas dos registos a 0, para passar para a proxima instrucao e ver quais os registos que ligam os Enables"""
  QPC = 0;
  QV = 0;
  QR = 0;
  QA = 0;
  QCYB = 0;
  QZ = 0;
  QOV = 0;
}

void carregarPrograma() {
  """Carrega o programa na memoria de codigo, cada posicao do array contem um certo endereco""",
  """sendo que a primeira instrucao comeca com o valor de 255 (MOV V #const8)"""
  memoriaCodigo[0] = 0x01FF; ///MOV V, #255
  memoriaCodigo[1] = 0x5;//MOV R, #5
  //  memoriaCodigo[2] = 0x3; //MOV @R, V
  //  memoriaCodigo[3] = 0x4; //MOV V, #0
  //  memoriaCodigo[4] = 0x6; //MOV V, @R = 255
  //  memoriaCodigo[5] = 0x3; //MOV V, #5
  //  memoriaCodigo[6] = 0x5;//MOV A, V
  //  memoriaCodigo[7] = 0x5; //MOV V, #0
  //  memoriaCodigo[8] = 0xD; //JZ 31
  //  memoriaCodigo[40] = 0x3; //MOV V, #255
  //  memoriaCodigo[41] = 0x5; // MOV A,V
  //  memoriaCodigo[42] = 0x9; //ADD, 255 + 255 (com Cy) falta adicionar o carry!
  //  memoriaCodigo[43] = 0xE; // JOV
  //  memoriaCodigo[44] = 0xC; //JNC 5
  //  memoriaCodigo[51] = 0xA; //SUB, 510 - 255 (com Bw) falta adicionar o borrow!
  //  memoriaCodigo[52] = 0x4; //MOV V, #5
  //  memoriaCodigo[53] = 0x5; // MOV A,V
  //  memoriaCodigo[54] = 0x4; //MOV V, #255
  memoriaCodigo[55] = 0x8; // NOR
  memoriaCodigo[60] = 0xF; //JMP end 0
}

void MCComb() {
  //ADRESSES
  """Funcao que correspondo a memoria de codigo, dentro dela irao estar os valores do AB e do DB"""
  """Cada digito ira corresponder a um certo bit, em uma potencia de 2^n"""
  """Os enables irao ser iguais aos valores da eprom, colocando o valor do endereco , e aplicando uma mascara para os enables deslocando um certo numero de casas para a direita"""
  AD0 = QZ;
  AD1 = QCYB;
  AD2 = (memoriaCodigo[QPC] & 0x040) >> 5; //6 bit do indice 0x040h(64)
  AD3 = (memoriaCodigo[QPC] & 0x080) >> 6; //7 bit do indice 0x080h(128)
  adress = AD0;
  adress = (AD1 << 1) | adress;
  adress = (AD2 << 2) | adress;
  adress = (AD3 << 3) | adress;
  //DATA
  ENOV = (EPROM_MC[adress] & 0x800) >> 11;
  ENCB = (EPROM_MC[adress] & 0x400) >> 10;
  ENZ = (EPROM_MC[adress] & 0x400) >> 9;
  WR = (EPROM_MC[adress] & 0x200) >> 8;
  RD = (EPROM_MC[adress] & 0x100) >> 7;
  ENV = (EPROM_MC[adress] & 0x080) >> 6;
  ENR = (EPROM_MC[adress] & 0x040) >> 5;
  ENA = (EPROM_MC[adress] & 0x020) >> 4;
  SPC0 = (EPROM_MC[adress] & 0x010) >> 3;
  SPC1 = (EPROM_MC[adress] & 0x008) >> 2;
  SA0 = (EPROM_MC[adress] & 0x004) >> 1;
  SA1 = (EPROM_MC[adress] & 0x002);
}

void MFComb() {
  """Funcao que corresponde ao modulo funcional da memoria de dados, onde estao representadas os simbolos WR e RD"""
  """Tambem ira ser guardado o valor da constante de 8 e da constante de 6"""
  """Cada registo feito e de acordo com o modulo funcional desenhado, das entradas dos registos, na maior parte recebem os muxs"""
  if (!WR) {
    QV = memoriaDados[QR];
  }

  if (!RD) {
    memoriaDados[QR] = QV;
  }

  DBC = memoriaCodigo[QPC];
  ABC = QPC;
  DBD = memoriaDados[QR];
  ABD = QR;

  //constante 8
  byte c0 =  memoriaDados[QPC] & 0x001;
  byte c1 = (memoriaDados[QPC] & 0x002) >> 1;
  byte c2 = (memoriaDados[QPC] & 0x004) >> 2;
  byte c3 = (memoriaDados[QPC] & 0x008) >> 3;
  byte c4 = (memoriaDados[QPC] & 0x010) >> 4;
  byte c5 = (memoriaDados[QPC] & 0x020) >> 5;
  byte c6 = (memoriaDados[QPC] & 0x040) >> 6;
  byte c7 = (memoriaDados[QPC] & 0x080) >> 7;
  const8 = c0;
  const8 = (c1 << 1) | const8;
  const8 = (c2 << 2) | const8;
  const8 = (c3 << 3) | const8;
  const8 = (c4 << 4) | const8;
  const8 = (c5 << 5) | const8;
  const8 = (c6 << 6) | const8;
  const8 = (c7 << 7) | const8;
  //constante 6
  byte C0 =  memoriaDados[QPC] & 0x001;
  byte C1 = (memoriaDados[QPC] & 0x002) >> 1;
  byte C2 = (memoriaDados[QPC] & 0x004) >> 2;
  byte C3 = (memoriaDados[QPC] & 0x008) >> 3;
  byte C4 = (memoriaDados[QPC] & 0x010) >> 4;
  byte C5 = (memoriaDados[QPC] & 0x020) >> 5;
  const6 = C0;
  const6 = (C1 << 1) | const6;
  const6 = (C2 << 2) | const6;
  const6 = (C3 << 3) | const6;
  const6 = (C4 << 4) | const6;
  const6 = (C5 << 5) | const6;

  //REGISTOS
  DSum = soma(ABC, Mux2x1(SPC0, 1, DBC & 0x01F)); // JNC/JZ rel5
  DPC = Mux2x1(SPC1, DSum, (DBC & 0x03F)); //JMP end6
  int D6 = (memoriaCodigo[QPC] & 0x040) >> 6; //Bits controlo ALU
  int D7 = (memoriaCodigo[QPC] & 0x080) >> 7;
  AUY = Mux4x2(D7, D6, nor(QV, QA), (QV + QA + QCYB), (QV - QA - QCYB), 0);
  DV = Mux4x2(SA1, SA0, 0, DBD, AUY, const8);
  DR = const6;
  DA = QR;


  //FLAGS
  if (AUY == (QV + QA)) {
    DCY = CySoma(QV, QA);
  }

  else if (AUY == (QV - QA)) {
    DCY = BwSub(QV, QA);
  }

  else {
    DCY = 0;
  }

  DZ = FZero(QV, QA);
}

void MFSequencial() {
  """Funcai que corresponde a cada registo apresentado no modulo funcional (Registo A,Pc,R,V,Z,OV e Cy)"""
  QPC = Registo(1, QPC, DPC);
  QV = Registo(ENV, QV, DV);
  QR = Registo(ENR, QR, DR);
  QA = Registo(ENA, QA, DA);
  QCYB = Registo(ENCB, QCYB, DCY);
  QZ = Registo(ENZ, QZ, DZ);
  QOV = Registo(ENOV, QOV, DOV);
}

void imprimirRegistos() {
  """Funcao que imprime os registos, quando as variaveis encontram-se no respetivo endereco, e feita a impressa dessa instrucao"""
  """Tambem imprime os valores do AB e do DB de ambas as memorias"""
  """Apresenta tambem os valores das saidas do modulo de controlo"""
  Serial.println("---REGISTOS---");
  Serial.print("QPC="); Serial.print(QPC); Serial.print(" "); Serial.print(adress >= 2 & adress <= 3 ? "JNC" : adress >= 4 & adress <= 7 ? "JZ" : adress >= 8 & adress <= 11 ? "JOV" :
      adress >= 8 & adress <= 11 ? "JOV" : adress >= 16 & adress <= 31 ? "JMP" : adress >= 32 & adress <= 35 ? "MOV V, #const8 " : adress >= 36 & adress <= 39 ? "MOV R, #const6"
      : adress >= 40 & adress <= 43 ? "MOV A,V " : adress >= 44 & adress <= 47 ? "MOV V, @R " : adress >= 48 & adress <= 51 ? "MOV @R,V " : adress >= 52 & adress <= 55 ? "NOR"
      : adress >= 56 & adress <= 59 ? "ADC " : adress >= 60 & adress <= 63 ? "SBB" : adress >= 0 & adress <= 1 ? "Nenhuma operação executada " : 0);

  if (adress >= 48 & adress <= 63) {
    if (QV == -1) {
      QV = 0;
      Serial.print(QV);

    }
    Serial.print(QV);
    Serial.print(" ");
    Serial.println(QA);
  }
  else if (adress >= 2 & adress <= 7) {
    Serial.println(DBC & 0x01F);
  }
  else if (adress >= 8 & adress <= 11) {
    Serial.println(DBC & 0x03F);
  }
  else if ((adress >= 16 & adress <= 31) || (adress >= 40 & adress <= 43)) {
    Serial.println(QV);
  }
  else if (adress >= 32 & adress <= 35) {
    Serial.println(QR);
  }
  else if (adress >= 36 & adress <= 39) {
    Serial.println(QA);
  }
  else if (adress >= 44 & adress <= 47) {
    Serial.println(DBD);
  }
  else if (adress >= 0 & adress <= 1) {

    Serial.println(" ");
  }
  Serial.print("QV="); Serial.println(adress >= 60 && adress <= 63 && QV == -1 ? QV = 0 : QV);
  Serial.print("QR="); Serial.println(QR);
  Serial.print("QA="); Serial.println(QA);
  Serial.print("ENV "); Serial.print("ENR "); Serial.print("ENA "); Serial.print("PC0 "); Serial.print("PC1 "); Serial.print("SA0 "); Serial.print("SA1 ");
  Serial.println("");
  Serial.print(" "); Serial.print(ENV); Serial.print(" "); Serial.print(ENR); Serial.print(" "); Serial.print(ENA); Serial.print(" "); Serial.print(SPC0); Serial.print(" ");
  Serial.print(SPC1); Serial.print(" ");
  Serial.print(SA0); Serial.print(" "); Serial.print(SA1); Serial.print(" ");
  Serial.println("");
  Serial.print("DBD="); Serial.print(DBD); Serial.print(" |ABD="); Serial.print(ABD); Serial.print(" |DBC="); Serial.print(DBC);
  Serial.print(" |ABC="); Serial.print(ABC); Serial.print(" |JNC="); Serial.print(QCYB); Serial.print(" |JZ="); Serial.print(QZ);
  Serial.println("");
  Serial.print("Memoria de dados 5 primeiros "); Serial.print(memoriaDados[0]); Serial.print(" "); Serial.print(memoriaDados[1]); Serial.print(" ");
  Serial.print(memoriaDados[2]); Serial.print(" "); Serial.print(memoriaDados[3]); Serial.print(" "); Serial.print(memoriaDados[4]); Serial.print(" "); Serial.println(memoriaDados[5]);
}
//----------------------------------------MUXS--------------------------------
byte Mux2x1(boolean Sel, byte E0, byte E1) {
  """Funcao que representa um mux2x1"""
  if (Sel) {
    return E1;
  }
  else {
    return E0;
  }
}

int Mux4x2(boolean S1, boolean S0, int A, int B, int C, int D) {
  """Funcao que representa um mux de 4x2"""
  switch (S1 << 1 | S0) {
    case B00:
      return A;
    case B01:
      return B;
    case B10:
      return C;
    case B11:
      return D;
  }
}

//----------------------------------------REGISTOS------------------------------
int Registo(boolean EN, int Q, int D) {
  """Funcao que implementea um corpo do registo, caso o En esteja a 1, ele entra nesse registo, caso contrario (En a 0) esse valor sai do registo"""
  if (EN) {
    return D;
  }
  else {
    return Q;
  }
}

int soma(int A, int B) {
  """Funcao que realiza o modulo soma"""
  return A + B;
}

int CySoma(int A, int B) {
  """Funcao que guarda o valor do carry, caso a soma dos dois operandos for maior que 255 (2^8) retorna um carry de 1, caso nao seja retorna 0"""
  if (A + B > 255) {
    return 1;
  }
  return 0;
}

int BwSub(int A, int B) {
  """Funcao que retorna o valor do borrow da subtracao, caso o valor da subtracao foi maior que o de A, e retornado o valor de 1 (bw a 1) senao e 0 (bw a 0)"""
  if (B > A) {
  return 1;
}
  return 0;
}

  int nor(int A, int B) {
  """Funcao que implementa o operador logica nor, aplicando uma mascara e com um operandor and bit a bit"""
  int s = (~(A | B)) & 0x0FF;
  return s;
}

int FZero(int A, int B) {
  """Funcao que implementa a flag zero, caso ambos as entradas da AU forem 0, retorna uma flag de 1, caso contrario 0"""
  int s = (~(A | B)) & 0x0FF;
  if (A == 0 || B == 0 || s == 0) {
    return 1;
  }
  return 0;
}
