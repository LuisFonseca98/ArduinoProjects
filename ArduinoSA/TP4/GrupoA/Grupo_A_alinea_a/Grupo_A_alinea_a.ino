#define pinoAnalogico 2;
#define pinoDigital 4;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,INPUT);
pinMode(4,INPUT);
Serial.println(taR());
Serial.println(tdR());
}

void loop() {

}

int taR(){
  int tempoInicial = micros();
  int tempoDR = analogRead(2);
  int tempoFinal = micros();
  int Duracao = tempoFinal - tempoInicial;
  return Duracao;
}
int tdR(){
  int tempoInicial = micros();
  int tempoDR = digitalRead(4);
  int tempoFinal= micros();
  int Duracao = tempoFinal - tempoInicial;
  return Duracao;
}

bool timeOver(unsigned long timeStamp,int timeInterval){
  return true;
  }
