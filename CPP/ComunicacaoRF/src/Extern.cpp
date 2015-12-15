/*
 * Extern.cpp
 *
 *  Created on: 06/12/2015
 *      Author: Airton Ribeiro
 */

#include "Extern.h"
#include "Comunicacao.h"

struct InfoRF {
	short temp, umid, valLdr, valSom, rfid;
} infoRF;

Comunicacao com = NULL;

int iniciar(char* porta) {
	com = Comunicacao(porta);
	return com.iniciar();
}
int ler() {
	int resultado = EXIT_FAILURE;
	char ci, cf;

	if (com.ler((char*) &ci, sizeof(ci)) == 0 && (ci == 'I')) {
		if (com.ler((char*) &infoRF, sizeof(InfoRF)) == 0) {
			if (com.ler((char*) &cf, sizeof(cf)) == EXIT_SUCCESS
					&& (cf == 'F')) {
				resultado = EXIT_SUCCESS;
			}
		}
	}

	return resultado;
}
int getRfid() {
	return infoRF.rfid;
}
int getUmidade() {
	return infoRF.umid;
}
int getTemperatura() {
	return infoRF.temp;
}
int getLdr() {
	return infoRF.valLdr;
}
int getSom() {
	return infoRF.valSom;
}
int finalizar() {
	return com.finalizar();
}

