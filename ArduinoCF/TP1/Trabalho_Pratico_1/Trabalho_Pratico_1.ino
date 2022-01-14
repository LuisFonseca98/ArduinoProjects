
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

    //--------------------------SOMADOR-----------------------------
    int somador1bit(int A,int B, int CyIn, int *CyOut)
    {
      int S;
      S = A^B^CyIn;
      *CyOut = A & B | A & CyIn | B & CyIn;
      return S;
    }
    int somador4bits(int X3, int X2, int X1, int X0, int Y3, int Y2, int Y1, int Y0)
    {
      int Cyout; 
      int *pCyout;
      int S;
      pCyout= &Cyout;
      S= (somador1bit(X0, Y0, 0, pCyout)); //S= S0
      S= (somador1bit(X1, Y1, Cyout, pCyout) << 1) | S; //S= S1 | S0
      S= (somador1bit(X2, Y2, Cyout, pCyout) << 2) | S; //S= S2 | S1 | S0
      S= (somador1bit(X3, Y3, Cyout, pCyout) << 3) | S; //S= S3 | S2 | S1 | S0
      return S;
    }
    //--------------------------------------SUBTRATOR---------------------
    
    int subtrator1bit(int A, int B, int CyIn, int *CyOut)
    {
      int S;
      S = ~A & 0x01;
      *CyOut = (~A & B) | (CyIn & B) | (~A & CyIn);
      return A^B^CyIn; 
    }
    int subtrator4bits(int X3, int X2, int X1, int X0, int Y3, int Y2, int Y1, int Y0)
    {
      int CyOut;
      int *pCyOut;
      int S;
      pCyOut= &CyOut;
      S = (subtrator1bit(X0,Y0,0,pCyOut));
      S = (subtrator1bit(X1, Y1, CyOut, pCyOut) << 1) | S;
      S = (subtrator1bit(X2, Y2, CyOut, pCyOut) << 2) | S;
      S = (subtrator1bit(X3, Y3, CyOut, pCyOut) << 3) | S;
      //S = CyOut << 4 | S;
      return S;
    }
 
     //------------------------NAND--------------------------------
    
     int NAND1bit(int A, int B, int CyIn, int *CyOut)
     {
      int S;
      S = ~(A & B) & 0x01;
      return S;
     }
     int NAND4bits(int X3, int X2, int X1, int X0, int Y3, int Y2, int Y1, int Y0){
      int CyOut;
      int *pCyOut;
      int S;
      pCyOut= &CyOut;
      S = (NAND1bit(X0,Y0,0,pCyOut));
      S = (NAND1bit(X1, Y1, CyOut, pCyOut) << 1) | S;
      S = (NAND1bit(X2, Y2, CyOut, pCyOut) << 2) | S;
      S = (NAND1bit(X3, Y3, CyOut, pCyOut) << 3) | S;
      return S;
  }
  //------------------------------DIVISOR-----------------------

      int divisor1bit(int A, int B) {
        int R = A; 
        int Q = 0;
        for(int i=0; R>=B; i++){
          R-=B;
          Q=i;
        }
      }
      boolean controlador4bits(int R3, int R2, int R1, int R0,int Y3, int Y2, int Y1, int Y0){
        while(R3>=Y3 && R2>=Y2 && R1>=Y1 && R0>=Y0){
          return 1;
        }
      }
      int divisor4bits(int X3, int X2, int X1, int X0, int Y3, int Y2, int Y1, int Y0){
        int R;
        int R3; int R2; int R1; int R0;
        R3 = X3; R2 = X2; R1 = X1; R0 = X0;
        int S = 0;
        
        while(R3>=Y3 && R2>=Y2 && R1>=Y1 && R0>=Y0){
          R = subtrator4bits(R3, R2, R1, R0, Y3, Y2, Y1, Y0);
          S = S + 1;

        }
        return S; 
      }
    //-----------------------MUX------------------------------------
    
  byte MUX_4x1 (boolean C0, boolean C1, int X3, int X2, int X1, int X0, int Y3, int Y2, int Y1, int Y0){
      int A = somador4bits(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
      int B = subtrator4bits(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
      //int C = divisor4bits(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
      int D = NAND4bits(X0, X1, X2, X3, Y0, Y1, Y2, Y3);
      
      if (C1 == 0 && C0 == 0){
        return A;
      }
      else if (C1 == 0 && C0 == 1){
        return B;
      }
      else if (C1 == 1 && C0 == 0){
       // return C;
      }
      else if (C1 == 1 && C0 == 1){
        return D;
      }
  }
  
 void loop() {
        
      boolean X3=digitalRead(3);//laranja
      boolean X2=digitalRead(4);//laranja
      boolean X1=digitalRead(5);//laranja
      boolean X0=digitalRead(6);//laranja
      boolean Y3=digitalRead(7);//branco
      boolean Y2=digitalRead(8);//branco
      boolean Y1=digitalRead(9);//branco
      boolean Y0=digitalRead(10);//branco
      boolean C0=digitalRead(11);//amarelo
      boolean C1=digitalRead(12);//preto
      boolean BOTAO=digitalRead(13);
      if(BOTAO == HIGH){
        byte f = MUX_4x1(C0, C1, X3, X2, X1, X0, Y3, Y2, Y1, Y0);
    
        Serial.print("A=");Serial.print(X0);Serial.print(X1);Serial.print(X2);Serial.print(X3);Serial.print("           ");Serial.println("Funções: Somador-> C1=0 e C0=0; Subtrator-> C1=0 e C0=1");
        Serial.print("B=");Serial.print(Y0);Serial.print(Y1);Serial.print(Y2);Serial.print(Y3);Serial.print("                    ");Serial.println("Divisor-> C1=1 e C0=0; NAND-> C1=1 e C0=1");
        Serial.print("C1=");Serial.print(C1);Serial.print(" C0=");Serial.println(C0);
        Serial.println(C1==0 && C0==0 ? "Somador" : C1==0 && C0==1 ? "Subtrator" : C1==1 && C0==0 ? "Divisor" : C1==1 && C0==1 ? "NAND" : "");
        Serial.println("RESULTADOS:");
        Serial.print("S(10)="); Serial.println(f);
        Serial.print("S(16)="); Serial.println(f,16);
        Serial.print("S(2)="); Serial.println(f,2);
        Serial.println("-------------------------------------------------------------------------");
      }
 }
