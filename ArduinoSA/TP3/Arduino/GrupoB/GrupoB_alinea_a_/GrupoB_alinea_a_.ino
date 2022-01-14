#define BOTAO A0
#define LED 8
#define LDR A4

void setup() {
Serial.begin(9600);
pinMode(BOTAO,INPUT);
pinMode(LDR,INPUT);
pinMode(LED,OUTPUT);
}

void loop() {
  premirBotao();
}
int valorBotao(){
  int valBotao = analogRead(BOTAO);
  return valBotao;
}
bool estadoBotao() {
  bool estbotao = false;
  if (valorBotao () == 1023){
    return true;
  }
  else{
    return false;
  }
  return true;
}

void premirBotao(){
   while (estadoBotao()==true){
    int ldrRead = analogRead(LDR);

  if(ldrRead >= 900){
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED,LOW);
    }
  }
}
