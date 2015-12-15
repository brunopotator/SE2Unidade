/*
 * Executor.cpp
 *
 *  Created on: 06/12/2015
 *      Author: Airton Ribeiro
 */

#include <iostream>
#include "Comunicacao.h"
using namespace std;

struct InfoRF {
	short temp, umid, valLdr, valSom, rfid;
};

int main(int argc, char **argv) {
	InfoRF sensor;
	Comunicacao com("COM3");
	com.iniciar();

	while (true) {
		char ci, cf;
		int resultado = com.ler((char*) &ci, sizeof(ci));
		if (resultado == EXIT_SUCCESS && (ci == 'I')) {
			//Se ocorrer tudo bem, Ler os sensores
			resultado = com.ler((char*) &sensor, sizeof(sensor));
			if (resultado == EXIT_SUCCESS) {
				//Se ler os eixos e carateres ocorrer bem
				//Ler o caractere F Final
				resultado = com.ler((char*) &cf, sizeof(cf));
				if ((resultado == EXIT_SUCCESS) && (cf == 'F')) {
					//Se tudo for atendido exibe os sensores lidos
					cout << "Rfid = " << sensor.rfid << endl;
					cout << "Temp = " << sensor.temp << endl;
					cout << "Umid = " << sensor.umid << endl;
					cout << "Ldr  = " << sensor.valLdr << endl;
					cout << "Som  = " << sensor.valSom << endl;
				}
			}
			Sleep(50);
		}
	}

		return 0;
	}
