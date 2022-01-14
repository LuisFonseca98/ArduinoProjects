boolean imprimir;
int adress;
//SAIDAS MODULO CONTROLO
boolean ENC,ENZ,ENV,ENR,ENA;
boolean WR,RD,SPC0,SPC1,JMP,JZ,JC,SA0,SA1,SA2;
boolean AD0,AD1,AD2,AD3,AD4,AD5; //ADRESS
int memCodigo[256]; //64
int memDados[128] = {0,0,0}; //4096 = 128*16
int rel5, end6;
int const8, const6;
int DBC,ABC;
int DBD,ABD;
int DPC,QPC,DV,QV,DCY,DZ,QCY,QZ,DA,QA,DR,QR;
int DSum, ALUY, ALUR;
boolean Pclock;
unsigned long tempoClk;
const unsigned long TFILTRO = 250;
const int EPROM_MC[64] = {0x300,0x300,0xB10,0xB10,0x710,0x710,0x308,0x308,0x308,0x308,0x0300,0x300,0x300,0x300,0x383,0x383,0x383,0x383,0x383,0x383,0x383,0x383,
0x383,0x383,0x383,0x383,0x383,0x383,0x383,0x383,0x340,0x340,0x340,0x340,0x340,0x320,0x320,0x320,0x320,0x182,0x182,0x182,0x182,0x200,0x200,0x200,0x200,
0x381,0x381,0x381,0x381,0xB81,0xB81,0xB81,0xB81,0xB81,0xB81,0xB81,0xB81,0x7A5,0x7A5,0x7A5,0x7A5};
void setup(){
Serial.begin(9600);
pinMode(2,OUTPUT);
Pclock = digitalRead(2);
tempoClk = millis();
attachInterrupt(0,Clock,RISING);
imprimir = true;
reset();
loadprogram();
interrupts();
}
void Clock(){
if(Pclock == HIGH){
if(millis() - tempoClk >= TFILTRO){
tempoClk = millis();
//reset();
MFSequencial();
imprimir = true;
}
}
else{
Pclock = digitalRead(2);
}
}
void loop(){
//MCComb();
//MFComb();
if(imprimir){
imprimirRegistos();
MCComb();
MFComb();
imprimir = false;
}
}
void reset(){
QPC = 0;
QV = 0;
QR = 0;
QA = 0;
QCY = 0;
QZ = 0;
}
void loadprogram(){
memCodigo[0] = 0x1FF;//MOV V, #255
memCodigo[1] = 0x205;//MOV R, #5
memCodigo[2] = 0x2C0; //MOV @R, V
memCodigo[3] = 0x100; //MOV V, #0
memCodigo[4] = 0x280; //MOV V, @R = 255
memCodigo[5] = 0x105; //MOV V, #5
memCodigo[6] = 0x240;//MOV A, V
memCodigo[7] = 0x100; //MOV V, #0
memCodigo[8] = 0x3C0; //DIV 0, 5
memCodigo[9] = 0x07F; //JZ 31
memCodigo[40] = 0x1FF; //MOV V, #255
memCodigo[41] = 0x240; // MOV A,V
memCodigo[42] = 0x340; //ADD, 255 + 255
memCodigo[43] = 0x005; //JC 5
memCodigo[51] = 0x380; //SUB, 510 - 255
memCodigo[52] = 0x105; //MOV V, #5
memCodigo[53] = 0x240; // MOV A,V
memCodigo[54] = 0x1FF; //MOV V, #255
memCodigo[55] = 0x3C0; //DIV 255, 5 (para trocar flag de Z)
//memCodigo[56] = 0x300; // NAND
/*
memCodigo[55] = 0x101;//MOV V, #1
memCodigo[56] = 0x240;//MOV A, V
memCodigo[57] = 0x201;//MOV R, #1
*/
memCodigo[60] = 0x80; //JMP end 0
}
void MCComb(){
//ADRESSES
AD0 = QZ;
AD1 = QCY;
AD2 = (memCodigo[QPC] & 0x040) >> 5; //6 bit do indice 0x040h(64)
AD3 = (memCodigo[QPC] & 0x080) >> 6; //7 bit do indice 0x080h(128)
AD4 = (memCodigo[QPC] & 0x100) >> 7; //8 bit do indice 0x100h(256)
AD5 = (memCodigo[QPC] & 0x200) >> 8; //9 bit do incide 0x200h(512)
adress = AD0;
adress = (AD1 << 1) | adress;
adress = (AD2 << 2) | adress;
adress = (AD3 << 3) | adress;
adress = (AD4 << 4) | adress;
adress = (AD5 << 5) | adress;
//DATA
ENC = (EPROM_MC[adress] & 0x800) >> 11;
ENZ = (EPROM_MC[adress] & 0x400) >> 10;
WR = (EPROM_MC[adress] & 0x200) >> 9;
RD = (EPROM_MC[adress] & 0x100) >> 8;
ENV = (EPROM_MC[adress] & 0x080) >> 7;
ENR = (EPROM_MC[adress] & 0x040) >> 6;
ENA = (EPROM_MC[adress] & 0x020) >> 5;
SPC0 = (EPROM_MC[adress] & 0x010) >> 4;
SPC1 = (EPROM_MC[adress] & 0x008) >> 3;
SA0 = (EPROM_MC[adress] & 0x004) >> 2;
SA1 = (EPROM_MC[adress] & 0x002) >> 1;
SA2 = (EPROM_MC[adress] & 0x001);
}
void MFComb(){
if(!WR){
QV = memDados[QR];
}
if(!RD){
memDados[QR]= QV;
}
DBC = memCodigo[QPC];
ABC = QPC;
DBD = memDados[QR];
ABD = QR;

//constante 8
byte c0 = memCodigo[QPC] & 0x001;
byte c1 = (memCodigo[QPC] & 0x002) >> 1;
byte c2 = (memCodigo[QPC] & 0x004) >> 2;
byte c3 = (memCodigo[QPC] & 0x008) >> 3;
byte c4 = (memCodigo[QPC] & 0x010) >> 4;
byte c5 = (memCodigo[QPC] & 0x020) >> 5;
byte c6 = (memCodigo[QPC] & 0x040) >> 6;
byte c7 = (memCodigo[QPC] & 0x080) >> 7;
const8 = c0;
const8 = (c1 << 1) | const8;
const8 = (c2 << 2) | const8;
const8 = (c3 << 3) | const8;
const8 = (c4 << 4) | const8;
const8 = (c5 << 5) | const8;
const8 = (c6 << 6) | const8;
const8 = (c7 << 7) | const8;
//constante 6
byte C0 = memCodigo[QPC] & 0x001;
byte C1 = (memCodigo[QPC] & 0x002) >> 1;
byte C2 = (memCodigo[QPC] & 0x004) >> 2;
byte C3 = (memCodigo[QPC] & 0x008) >> 3;
byte C4 = (memCodigo[QPC] & 0x010) >> 4;
byte C5 = (memCodigo[QPC] & 0x020) >> 5;
const6 = C0;
const6 = (C1 << 1) | const6;
const6 = (C2 << 2) | const6;
const6 = (C3 << 3) | const6;
const6 = (C4 << 4) | const6;
const6 = (C5 << 5) | const6;

//REGISTOS
DSum = soma(ABC,Mux2x1(SPC0,1,DBC & 0x01F)); // JC/JZ rel5
DPC = Mux2x1(SPC1,DSum,(DBC & 0x03F)); //JMP end6
int D6 = (memCodigo[QPC] & 0x040) >> 6; //Bits controlo ALU
int D7 = (memCodigo[QPC] & 0x080) >> 7;
ALUY = Mux4x2(D7,D6,nand(QV,QA),(QV+QA),(QV-QA),(QV/QA));
DV = Mux4x2(SA2,SA1,0,DBD,ALUY,const8);
DR = const6;
DA = Mux2x1(SA0,QV,(QV%QA));

//FLAGS
if(ALUY == (QV+QA)){
DCY = CySoma(QV,QA);
}
else if (ALUY == (QV-QA)){
DCY = BwSub(QV,QA);
}
else{
DCY = 0;
}
DZ = FZero(QV,QA);
}
void MFSequencial(){
QPC = Registo(1,QPC,DPC);
QV = Registo(ENV,QV,DV);
QR = Registo(ENR,QR,DR);

QA = Registo(ENA,QA,DA);
QCY = Registo(ENC,QCY,DCY);
QZ = Registo(ENZ,QZ,DZ);
}
void imprimirRegistos(){
Serial.println("---REGISTOS---");
Serial.print("QPC=");Serial.print(QPC);Serial.print(" ");Serial.print( adress >= 2 & adress <= 3 ? "JC " : adress >= 4 & adress <= 7? "JZ " :
adress >= 8 & adress <= 11? "JMP " : adress>=16 & adress<=31 ? "MOV V, #const8 " : adress >=32 & adress <=35? "MOV R, #const6 " : adress >= 36 & adress <=39 ? "MOV A, V "
: adress >= 40 & adress <= 43? "MOV V, @R " : adress >= 44 & adress <= 47? "MOV @R, V " : adress >= 48 & adress <= 51 ? "NAND " : adress >= 52 & adress <= 55 ? "ADD "
: adress >= 56 & adress <= 59? "SUB " : adress >= 60 & adress <= 63? "DIV " : adress >= 0 & adress <= 1 ? "Nenhuma operação executada " : 0);
if(adress >= 48 & adress <= 63){
if(QV==-1){
QV = 0;
Serial.print(QV);

}
Serial.print(QV);
Serial.print(" ");
Serial.println(QA);
}
else if(adress >= 2 & adress <= 7){
Serial.println(DBC & 0x01F);
}
else if(adress >= 8 & adress <= 11){
Serial.println(DBC & 0x03F);
}
else if((adress >= 16 & adress <= 31) || (adress >= 40 & adress <= 43)){
Serial.println(QV);
}
else if(adress >= 32 & adress <= 35){
Serial.println(QR);
}
else if(adress >= 36 & adress <= 39){
Serial.println(QA);
}
else if(adress >= 44 & adress <= 47){
Serial.println(DBD);
}
else if(adress >= 0 & adress <= 1){

Serial.println(" ");
}
Serial.print("QV=");Serial.println(adress >= 60 && adress <= 63 && QV == -1 ? QV = 0 : QV);
Serial.print("QR=");Serial.println(QR);
Serial.print("QA=");Serial.println(QA);
Serial.print("ENV ");Serial.print("ENR ");Serial.print("ENA ");Serial.print("PC0 ");Serial.print("PC1 ");Serial.print("SA0 ");Serial.print("SA1 ");Serial.print("SA2 ");
Serial.println("");
Serial.print(" ");Serial.print(ENV);Serial.print(" ");Serial.print(ENR);Serial.print(" ");Serial.print(ENA);Serial.print(" ");Serial.print(SPC0);Serial.print(" ");
Serial.print(SPC1);Serial.print(" ");
Serial.print(SA0);Serial.print(" ");Serial.print(SA1);Serial.print(" "); Serial.print(SA2);Serial.print(" ");
Serial.println("");
Serial.print("DBD=");Serial.print(DBD);Serial.print(" |ABD=");Serial.print(ABD);Serial.print(" |DBC=");Serial.print(DBC);
Serial.print(" |ABC=");Serial.print(ABC);Serial.print(" |JC=");Serial.print(QCY);Serial.print(" |JZ=");Serial.print(QZ);
Serial.println("");
Serial.print("Memoria de dados 5 primeiros ");Serial.print(memDados[0]);Serial.print(" ");Serial.print(memDados[1]);Serial.print(" ");Serial.print(memDados[2]);Serial.print(" ");Serial.print(memDados[3]);Serial.print(" ");Serial.print(memDados[4]);Serial.print(" ");Serial.println(memDados[5]);
}
int Registo(boolean EN,int Q,int D){
if(EN){
return D;
}
else{
return Q;
}
}
byte Mux2x1(boolean Sel, byte E0, byte E1){
if(Sel){
return E1;
}
else{
return E0;
}
}
int Mux4x2(boolean S1, boolean S0, int A, int B, int C, int D){ //USAR NA ALU
switch (S1 << 1 | S0){
case B00:
return A;
case B01:
return B;
case B10:
return C;
case B11:
return D;
}
}
int soma(int A, int B){
return A + B ;
}
int CySoma(int A, int B){
if (A+B > 255){
return 1;
}
return 0;
}
int BwSub(int A, int B){
if(B > A){
return 1;
}
return 0;
}
int nand(int A, int B){
int s = (~(A & B)) & 0x0FF;
return s;
}
int FZero(int A, int B){
int s = (~(A & B)) & 0x0FF;
if(A == 0 || B == 0 || s == 0){
return 1;
}
return 0;
}
