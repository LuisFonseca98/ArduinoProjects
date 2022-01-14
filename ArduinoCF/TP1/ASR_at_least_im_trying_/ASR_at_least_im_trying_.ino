
bool A[] = {1, 0, 1, 0}; //0010 //entrada A que contem 4bits
bool B[] = {1, 1, 0, 1}; //0010 //entrada B que contem 4bits
bool D;
bool S[4];
bool Z;
volatile boolean Q; // Saída Q
volatile boolean EnX,EnY,SelX,SelY; //


void setup() {
Serial.begin(9600);
attachInterrupt(digitalPinToInterrupt(0), CLK, RISING);
interrupts();
}

void loop() {}

  boolean flip_flop_D(boolean D){return D;}
  void CLK(){ Q = flip_flop_D(D);}



bool mux2x1(bool A, bool B, bool C){
  return (!C) ? A0:B; 
}

bool mux4x2(bool AA, bool BB, bool CC, bool DD, bool E1, bool E0){
  return (!E1 && !E0 && AA || !E1&&E0&&BB || E1&&E0&&CC || E1&&E0&&DD);
}

void shiftRight(bool aa3, bool aa2, bool aa1, bool aa0, bool bb3, bool bb2, bool bb1, bool bb0){
  bool SS0 = mux4x2(aa0,aa1,aa2,aa3,bb1,bb0);
  bool SS1 = mux4x2(aa1,aa2,aa3,0,bb1,bb0);
  bool SS2 = mux4x2(aa2,aa3,0,0,bb1,bb0);
  bool SS3 = mux4x2(aa3,0,0,0,bb1,bb0);

  S[0] = mux2x1(SS0,0,bb2 || bb3);
  S[1] = mux2x1(SS1,0,bb2 || bb3);
  S[2] = mux2x1(SS2,0,bb2 || bb3);
  S[3] = mux2x1(SS3,0,bb2 || bb3);

  Z = mux2x1(1,0,(S[0] || S[1] || S[2] || S[3]));
}
