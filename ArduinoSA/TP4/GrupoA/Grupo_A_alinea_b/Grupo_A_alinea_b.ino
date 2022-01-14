void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

}

bool timeOver(unsigned long timeStamp,int timeInterval){
  unsigned int tempo = micro();
  unsigned int val = tempo - timeStamp;
  if(val >= 0) return 0;
  if(val >= timeIntervel){
    return true;
  }
  return false;
  }
