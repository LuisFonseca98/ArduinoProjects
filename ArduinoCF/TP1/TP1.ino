bool A[4]; //0010 //entrada A que contem 4bits
bool B[4]; //0010 //entrada B que contem 4bits
bool Carry_Soma[4];
bool Carry_Sub[4];
bool S[4]; //saida que contem os valores dos 4 bits, mais o Cy e o Borrow
bool Cy;
bool Of;
bool C1;
bool C0;
int a_0 = 4;
int a_1 = 5;
int a_2 = 6;
int a_3 = 7;
int b_0 = 8;
int b_1 = 9;
int b_2 = 10;
int b_3 = 11;
int S0 = 19;
int S1 = 18;
int S2 = 17;
int S3 = 16;

void setup() {
  // put your setup code here, to run once:
  //FLAGS
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  //BITS CONTROLO
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  //SAIDA
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  //ENTRADAS
  pinMode(a_0, INPUT);
  pinMode(a_1, INPUT);
  pinMode(a_2, INPUT);
  pinMode(a_3, INPUT);
  pinMode(b_0, INPUT);
  pinMode(b_1, INPUT);
  pinMode(b_2, INPUT);
  pinMode(b_3, INPUT);




}

void loop() {
  // put your main code here, to run repeatedly:
//  escreverEntradas();
  escreverEntradas();
  mostrarA();
  mostrarB();
  ALU(A, B, C1, C0);
  escreverSaidas();

  while (true);
}

//----------------------ENTRADAS--------------------------------
  void mostrarA() {
    Serial.print("A- ");
  for (int i = 0; i < 4; i++) {
    Serial.print(A[i]);
    }
  }

  void mostrarB() {
    Serial.print(" B- ");
    for (int i = 0; i < 4; i++) {
      Serial.print(B[i]);
    }
  }

void escreverEntradas(){
  C1=digitalRead(12);
  C0=digitalRead(13);
  A[0]=digitalRead(a_0);
  A[1]=digitalRead(a_1);
  A[2]=digitalRead(a_2);
  A[3]=digitalRead(a_3);
  B[0]=digitalRead(b_0);
  B[1]=digitalRead(b_1);
  B[2]=digitalRead(b_2);
  B[3]=digitalRead(b_3);
}
//----------------------------MÓDULOS--------------------------
bool somador_1_bit_S(bool A, bool B, bool CyIN) {
  bool S;
  Cy = CyIN;

  S = A ^ B ^ Cy;

  return S;
}

bool somador_1_bit_C(bool A, bool B, bool CyIN) {
  bool Carry;
  Cy = CyIN;

  Carry = A & B |  A & Cy  | B & Cy;

  return Carry;
}


void moduloSomador(bool A[], bool B[]) {

 
  Carry_Soma[3] = somador_1_bit_C(A[3], B[3], 0);
  Carry_Soma[2] = somador_1_bit_C(A[2], B[2], Carry_Soma[3]);
  Carry_Soma[1] = somador_1_bit_C(A[1], B[1], Carry_Soma[2]);
  Carry_Soma[0] = somador_1_bit_C(A[0], B[0], Carry_Soma[1]);

  Cy = Carry_Soma[0];

}

void moduloSubtrator(bool A[], bool B[]) {


  Carry_Sub[3] = somador_1_bit_C(A[3], !B[3], 1);
  Carry_Sub[2] = somador_1_bit_C(A[2], !B[2], Carry_Sub[3]);
  Carry_Sub[1] = somador_1_bit_C(A[1], !B[1], Carry_Sub[2]);
  Carry_Sub[0] = somador_1_bit_C(A[0], !B[0], Carry_Sub[1]);
  
  Cy = !Carry_Sub[0];

}

bool NOR_1bit(bool A, bool B) {
  bool S;
  S = !(A | B);

  return S;
}

void moduloNOR(bool A[], bool B[]) {

  NOR_1bit(A[0], B[0]);
  NOR_1bit(A[1], B[1]);
  NOR_1bit(A[2], B[2]);
  NOR_1bit(A[3], B[3]);

}

//----------------------ALU----------------------------------


void ALU(bool A[], bool B[], bool C1, bool C0) {
  moduloSomador(A, B);
  moduloSubtrator(A, B);
  moduloNOR(A, B);

}

bool MUX_S3 (bool A, bool B, bool C1, bool C0) {
  return !C1 & !C0 & somador_1_bit_S(A, B,0)| !C1 & C0 & somador_1_bit_S(A, !B, 1) | C1 & C0 & NOR_1bit(A,B);
}

bool MUX_S2 (bool A, bool B, bool C1, bool C0) {
  return !C1 & !C0 & somador_1_bit_S(A, B,Carry_Soma[3])| !C1 & C0 & somador_1_bit_S(A, !B, Carry_Sub[3]) | C1 & C0 & NOR_1bit(A,B);
}

bool MUX_S1 (bool A, bool B, bool C1, bool C0) {
  return !C1 & !C0 & somador_1_bit_S(A, B,Carry_Soma[2])| !C1 & C0 & somador_1_bit_S(A, !B, Carry_Sub[2]) | C1 & C0 & NOR_1bit(A,B);
}

bool MUX_S0 (bool A, bool B, bool C1, bool C0) {
  return !C1 & !C0 & somador_1_bit_S(A, B,Carry_Soma[1])| !C1 & C0 & somador_1_bit_S(A, !B, Carry_Sub[1]) | C1 & C0 & NOR_1bit(A,B);
}

//----------------------SAÍDAS----------------------------------
void escreverSaidas() {

  MUX_Saidas(C1,C0);

}

void MUX_Saidas (bool C1, bool C0) {
  switch (C1 << 1 | C0) {
    case B00:
      mostrarResultado();
      flagZero(S);
      mostrarCarry();
      mostrarOverflow();
      break;
    case B01:
      flagZero(S);
      mostrarResultado();
      mostrarBorrow();
      mostrarOverflow_sub();
      break;
    case B11:
      mostrarResultado();
      flagZero(S);
      break;
  }
}

//----------------------RESULTADOS-E-FLAGS----------------------------------

void mostrarResultado() {
  S[0]=MUX_S0(A[0], B[0], C1, C0);
  S[1]=MUX_S1(A[1], B[1], C1, C0);
  S[2]=MUX_S2(A[2], B[2], C1, C0);
  S[3]=MUX_S3(A[3], B[3], C1, C0);
  //APAGAR
  Serial.println();
  Serial.print(S[0]);
  Serial.print(S[1]);
  Serial.print(S[2]);
  Serial.print(S[3]);
  //NAO APAGAR
  digitalWrite(S0, S[0]);
  digitalWrite(S1, S[1]);
  digitalWrite(S2, S[2]);
  digitalWrite(S3, S[3]);

}


void mostrarCarry() {
  Serial.println();
  Serial.print("Carry/Borrow: " );
  Serial.print(Carry_Soma[0]);
  digitalWrite(15, Carry_Soma[0]);
}

void mostrarBorrow() {
  Serial.println();
  Serial.print("Carry/Borrow: " );
  Serial.print(Carry_Soma[0]);
  digitalWrite(15, !Carry_Sub[0]);
}


void mostrarOverflow() {
  Of = XOR(Carry_Soma[1], Carry_Soma[0]);
  Serial.println();
  Serial.print("Overflow: " );
  Serial.print(Of);
  digitalWrite(14, Of);
}

void mostrarOverflow_sub() {
  Of = XOR(Carry_Sub[1], Carry_Sub[0]);
  Serial.println();
  Serial.print("Overflow: " );
  Serial.print(Of);
  digitalWrite(14, Of);
}

void flagZero(bool S[]) {
  Serial.println();
  Serial.print("Zero: ");
  Serial.print(NOR_flag(S));
  digitalWrite(3, NOR_flag(S));
}


//----------------------FUNÇÕES-AUXILIARES---------------------------------
bool NOR_flag(bool S[]) {

  return !(S[0] | S[1] | S[2] | S[3]);
}

bool XOR(bool A, bool B) {
  return (A & !B) | (B & !A);
}
