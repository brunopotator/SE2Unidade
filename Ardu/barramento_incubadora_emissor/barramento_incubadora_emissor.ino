#include "RCSwitch.h"
#include "DHT.h"

RCSwitch emissor = RCSwitch();

//RFID pode ser entre 0 e 32767 (15bits)
//FRPT bits para controle em transmissão em duas partes (1bits); Iremos usar parte 0 e parte 1
//TEMP, UMID, LDR, SOM passarão pelo map para limitar até 255 (8bits)
//Mesmo tendo tamanho iguais das variaveis nas partes 1 e 2, decidimos definir em variaveis separadas, caso ocorra necessidade de alterações
#define RFID 11012
#define DESLOCAMENTO_RFID 17
#define DESLOCAMENTO_RFPT 16
#define DESLOCAMENTO_TEMP 8
//#define DESLOCAMENTO_UMID 0
#define DESLOCAMENTO_LDR 8
//#define DESLOCAMENTO_SOM 0

//Pinos Analógicos
#define PINO_SOM 0          //Ligar Saida Sensor Som ky-038 ou Potenciometro
#define PINO_LDR 1          //Ligar Saida Sensor LDR ou Potenciometro
#define DHTPIN A2           // Ligar no Pino Analógico
#define DHTTYPE DHT11       // Modelo DHT11
DHT dht(DHTPIN, DHTTYPE);   // Definindo DHT

//Pinos Digitais
#define PINO_EMISSOR 4  //Ligar no pino digital

//Funcao para enviar long pelo RF
void emitir (unsigned long infoxx) {
  emissor.send(infoxx, 32);
}

//Função para gerar long com dados da 1 transmissão
unsigned long concatenarSensoresRF1( unsigned long temp, unsigned long umid) {
  unsigned long rf = RFID;
  unsigned long pt = 0; //Enviar Sensores PARTE 0 ou PARTE 1
  unsigned long info = rf << DESLOCAMENTO_RFID;
  info = info | (pt << DESLOCAMENTO_RFPT);
  info = info | (temp << DESLOCAMENTO_TEMP);
  info = info | umid;

  return info;
}

//Função para gerar long com dados da 2 transmissão
unsigned long concatenarSensoresRF2( unsigned long ldr, unsigned long som) {
  unsigned long rf = RFID;
  unsigned long pt = 1; //Enviar Sensores PARTE 0 ou PARTE 1
  unsigned long info = rf << DESLOCAMENTO_RFID;
  info = info | (pt << DESLOCAMENTO_RFPT);
  info = info | (ldr << DESLOCAMENTO_LDR);
  info = info | som;

  return info;
}

void setup() {
  Serial.begin(9600);
  //Configuração do Receptor
  emissor.enableTransmit(PINO_EMISSOR);

  //Configuração Sensores
  pinMode(PINO_LDR, INPUT);
  pinMode(PINO_SOM, INPUT);
  dht.begin();
}

void loop() {
  //Emissão de Dados
  unsigned long temp = dht.readTemperature();
  unsigned long umid = dht.readHumidity();
  unsigned long valLdr, valSom;
  valLdr = analogRead(PINO_LDR);
  valLdr = map(valLdr, 0, 1023, 0, 255);
  valSom = analogRead(PINO_SOM);
  valSom = map(valSom, 0, 1023, 0, 255);
  unsigned long info1 = concatenarSensoresRF1(temp, umid);
  unsigned long info2 = concatenarSensoresRF2(valLdr, valSom);
  Serial.print("Info1: "); Serial.println(info1); Serial.print("Info1BIN: "); Serial.println(info1, BIN);
  emitir(info1);
  delay(100);
  Serial.print("Info2: "); Serial.println(info2); Serial.print("Info2BIN: "); Serial.println(info2, BIN);
  emitir(info2);
  delay(100);
  //Teste Print
  //Serial.print("Info1: "); Serial.println(info1); Serial.print("Info2: "); Serial.println(info2);
  Serial.print("Temp: "); Serial.print(temp); Serial.print("\t\tUmid: "); Serial.println(umid); Serial.print("Ldr: "); Serial.print(valLdr); Serial.print("\t\t\tSom: "); Serial.println(valSom);

}
