#define POT A1
#define BOTAO A0

void setup() {
  Serial.begin(9600);
  pinMode(POT,INPUT);
  pinMode(BOTAO,INPUT);
}


void loop() {
  funcaoBotao();

}

int potRead10bits() {
  int valorPot = analogRead(A1); //0 a 1023
  return valorPot;
}

float potReadVolt() {
  float voltagem = (potRead10bits() * (5.0 / 1023)); // Tensao Maxima(5.00v)
  return voltagem;
}

float potReadRpot23() {
  float pot23 = (potReadpos () * (10.75 / 1023)); //(diferentes posicoes do potenciometro)
  return pot23;
}

int potReadpos() {
  int posPot = ((potRead10bits() * 0.098)); //diferentes posicoes
  return posPot;
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

void funcaoBotao() {
  if (estadoBotao()== true) {
  potDisp();
  
  //piscaLED3Vezes
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW);
  delay(1000);
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW);
  delay(1000);
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW);
  }
  else (estadoBotao() == false);
}

void potDisp() {
  Serial.println(String(potRead10bits()) + "\t" + String(potReadVolt()) + "\t" + String(potReadRpot23()) + "\t" + String(potReadpos()));
}
