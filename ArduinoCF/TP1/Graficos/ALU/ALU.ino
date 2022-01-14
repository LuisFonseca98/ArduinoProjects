void setup() {
  Serial.begin(9600);
}

void loop() {}

void divisor(int Enable,int Y,int X, int *Pq, int *Pr,int *pOK, int *pErro){
  int Q,R,OK,ERRO;
  Pq = &Q;  pOK = &OK;
  Pr = &R;  pErro = &ERRO;
  if(~Enable){
    Q = 0;
    R = X;
  }else{
    if(Y == 0){
      ERRO = 0;   OK = 1;
      while(R >= Y){
        Q++;
        R = R-Y;
      }
      OK = 1;
    }
  }
}

