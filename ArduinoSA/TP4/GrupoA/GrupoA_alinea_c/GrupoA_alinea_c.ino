#define PIN_DUTY 3
#define BOTAO A3
int estadoBotao = 0;

void setup() {
 pinMode(PIN_DUTY,OUTPUT);
 pinMode(BOTAO,INPUT);
 analogWrite(3,127);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(lerBotao() == true){
    int duty = 0;
    while(duty < 204){
      Serial.println(duty);
      analogWrite(PIN_DUTY,duty);
      delay(20);
      duty += 1;
    }
    delay(5000);
    while(duty >= 0){
      Serial.println(duty);
      analogWrite(PIN_DUTY,duty);
      delay(5);
      duty -= 1;
    }
  }
}

boolean lerBotao(){
  estadoBotao = digitalRead(BOTAO);
  if(estadoBotao == HIGH){
  return true;
  }else{
    return false;
  }
  return true;
}
