
#define POT A1
#define PIEZO A2
void setup() {
  pinMode(POT,INPUT);
  pinMode(PIEZO,OUTPUT);
  Serial.begin(9600);
}

void loop() {
Serial.println(String(potReadpos()));
diferentesFrequenciasPiezo();
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
// GrupoB alinea b) switch case para regular as diferentes posicoes
int diferentesFrequenciasPiezo(){
  int readPos = potReadpos();
  switch(readPos) {
    case 0 ... 20:
        tone(PIEZO,500,250);
        delay(250);
        break;
    case 21 ... 40:
        tone(PIEZO,400,125);
        delay(125);
        break;
    case 41 ... 60:
        tone(PIEZO,300,62.5);
        delay(62.5);
        break;
    case 61 ... 80:
        tone(PIEZO,200,31.25);
        delay(31.25);
        break;
    case 81 ... 100:
        tone(PIEZO,100,15.625);
        delay(15.625);
        break;
  }
}
