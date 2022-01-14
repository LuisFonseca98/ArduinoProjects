#define LDR 4
#define PIEZO 5

bool uartRead (char validChar){
  if (Serial.available()>0){
    char letra= Serial.available();
    if(letra==validChar){
      return true;
      }else{
        return false;
      }
  }
  return false;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(uartRead('p')){
  tone(PIEZO,200,2000);
  delay(1000);
  tone(PIEZO,300,2000);
  delay(1000);
  }
}

void loop() {
  ldrDisp();
  delay(1000);
}

int ldrRead10bits(){
  int valorLDR = analogRead(A4);
  return valorLDR;
}

float ldrReadVolt(){   
  float maxTensao =  (ldrRead10bits() *(5./1023));
  return maxTensao;
  }

float ldrReadRldr(){
  float resistenciaLDR = ((235/ldrReadVolt()) - 47);
  return resistenciaLDR;
}

int ldrReadLum(){
  int lumLDR = pow(ldrReadRldr()/10.47,-3.33333333);
  return lumLDR;
  }

void ldrDisp(){
Serial.println(String(ldrRead10bits()) + "\t" + String(ldrReadVolt()) + "\t" + String(ldrReadRldr()) + "\t" + String(ldrReadLum()));
}
  
