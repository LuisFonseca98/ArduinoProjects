#define BOTAO A4

void setup() {
Serial.begin(9600);
pinMode(A4, INPUT_PULLUP);
if(uartRead('g')){
  Serial.println("o carater foi recebido");
}else{
   Serial.println("foi recebido outro carater para alem de g");
  }
}

void loop() {
  Serial.println(buttonRead());
}
bool buttonRead(){
  boolean estadoBotao = digitalRead(BOTAO);
  if(BOTAO == LOW){
    return true;
  } else {
    return false;
  }
  return false;
}
bool uartRead (char validChar){
  if (Serial.available() > 0){
    char valSerial = Serial.read();
    if (valSerial == validChar){
      return true;
    }
    return false;
  }
  return true;
}
