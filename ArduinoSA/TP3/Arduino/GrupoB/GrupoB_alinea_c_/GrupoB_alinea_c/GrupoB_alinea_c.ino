#define LED 6
char operador; 
float n1,n2,resultado;

void setup() {
Serial.begin(9600);
pinMode(LED,OUTPUT);
Serial.println("Apresente uma conta");

}

void loop() {
while(Serial.available() > 0){
  n1 = Serial.parseInt();
  n2 = Serial.parseInt();
  operador = Serial.read();
  operacoesLED();
  }
  
}

void operacoesLED(){
  if(operador == '+'){
    resultado = n1 + n2;
    Serial.println("Resultado da adicao= ");
    Serial.print(resultado);
    Serial.println();
    for(int i = 0; i < resultado;i++){
      digitalWrite(LED,HIGH);
      delay(2000);
      digitalWrite(LED,LOW);
      delay(2000);
  }
}else if(operador == '-'){
  resultado = n1 - n2;
  Serial.println("Resultado da subtracao = ");
  Serial.println(resultado);
  for(int i = 0; i < resultado;i++){
    digitalWrite(LED,HIGH);
    delay(2000);
    digitalWrite(LED,LOW);
    delay(2000);
  }
}else if( operador == '*'){
  resultado = n1 * n2;
  Serial.println("Resultado da multiplicacao = ");
  Serial.println(resultado);
  for(int i = 0; i < resultado;i++){
    digitalWrite(LED,HIGH);
    delay(2000);
    digitalWrite(LED,LOW);
    delay(2000);
  }
}else if( operador == '/'){
  Serial.println("Resultado da divisao = ");
  Serial.println(resultado);
  for(int i = 0; i < resultado;i++){
    digitalWrite(LED,HIGH);
    delay(2000);
    digitalWrite(LED,LOW);
    delay(2000);
    }
  }
}
