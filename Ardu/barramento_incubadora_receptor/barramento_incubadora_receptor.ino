#include "RCSwitch.h"

RCSwitch receptor = RCSwitch();

//RFID pode ser entre 0 e 16383 (14bits)
//FRPT bits para controle em transmissão em até quatro partes (2bits); Iremos usar duas partes "01 e "10 = 1 e 2
//TEMP, UMID, LDR, SOM passarão pelo map para limitar até 255 (8bits)
//Mesmo tendo tamanho iguais das variaveis nas partes 1 e 2, decidimos definir em variaveis separadas, caso ocorra necessidade de alterações
#define RFID 11012
#define DESLOCAMENTO_RFID 17
#define DESLOCAMENTO_RFPT 16
#define DESLOCAMENTO_TEMP 8
//#define DESLOCAMENTO_UMID 0
#define DESLOCAMENTO_LDR 8
//#define DESLOCAMENTO_SOM 0

//Pinos Digitais
#define PINO_RECEPTOR 0     //0 = Primeiro pino de Interrupção; No Arduino Uno R3/Nano =  Pino Digital nº 2

//Variaveis Globais
unsigned long info1;
unsigned long info2;

struct Sensores { // necessário inserir todos os eixos dos sensores
  int temp, umid, valLdr, valSom, rfid;
};
Sensores sensor;

//Refazer função recebimento, será em duas partes
//Retorno será feito em passagem por referencia ponteiro(*nm_variavel) endereço(&nm_variavel) testar como usar no c do arduino
void receber() {
  unsigned long info;

  if (receptor.available()) {
    info = receptor.getReceivedValue();
    //Serial.print("Info_Recebido: ");
    //Serial.println(info, BIN);
    receptor.resetAvailable();
  }

  if (RFIDValido(info)) {
    if (RFP1Valido(info)) {
      info1 = info;
    }
    else if (RFP2Valido(info)) {
      info2 = info;
    }
    //else {*info1 = -1; *info2 = -1;}

  }
}

boolean RFIDValido(long info) {
  boolean valido = false;

  int rfid = info >> DESLOCAMENTO_RFID;
  if (rfid == RFID) {
    valido = true;
  }
  return valido;
}

boolean RFP1Valido(long info) {
  boolean valido = false;

  int rfpt = (info & 65536) >> DESLOCAMENTO_RFPT;
  if (rfpt == 0) {
    valido = true;
  }
  return valido;
}

boolean RFP2Valido(long info) {
  boolean valido = false;

  int rfpt = (info & 65536) >> DESLOCAMENTO_RFPT;
  if (rfpt == 1) {
    valido = true;
  }
  return valido;
}

int extrairRfid(long info) {
  int rfid = info >> DESLOCAMENTO_RFID;
  return rfid;
}

int extrairTemp(long info) {
  int temp = (info & 65280) >> DESLOCAMENTO_TEMP;
  return temp;
}

int extrairUmid(long info) {
  int umid = (info & 255);
  return umid;
}

int extrairLdr(long info) {
  int ldr = (info & 65280) >> DESLOCAMENTO_LDR;
  return ldr;
}

int extrairSom(long info) {
  int som = (info & 255);
  return som;
}

void enviarSensores() {
  int tam = sizeof(sensor); // retorna o tamanho total em bytes
  char buff[tam]; // cria um buffer que recebe o tamanho

  memcpy(&buff, &sensor, tam); // envia o endereço do buffer, o endereço dos eixos e o tamanho total

  Serial.write('I'); // inicializar
  Serial.write((uint8_t*)&buff, tam); // converte o buff para
  Serial.write('F'); // finalizar
}

void setup() {
  Serial.begin(9600);
  //Configuração do Receptor
  receptor.enableReceive(PINO_RECEPTOR);
  info1 = -1;
  info2 = -1;
  //Serial.print(sizeof(sensor)); Tamanho de sensor = 10
}

void loop() {
  //Emissão de Dados
  receber();
  delay(250);
  //Serial.print("Info1: "); Serial.println(info1); Serial.print("Info2: "); Serial.println(info2);
  sensor.rfid = extrairRfid(info1);
  sensor.temp = extrairTemp(info1);
  sensor.umid = extrairUmid(info1);
  sensor.valLdr = extrairLdr(info2);
  sensor.valSom = extrairSom(info2);
  
  enviarSensores();
  //Serial.print("Temp: "); Serial.print(sensor.temp); Serial.print("\t\tUmid: "); Serial.println(sensor.umid); Serial.print("Ldr: "); Serial.print(sensor.valLdr); Serial.print("\t\t\tSom: "); Serial.println(sensor.valSom);
}
