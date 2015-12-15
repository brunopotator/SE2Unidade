/*
 * Extern.h
 *
 *  Created on: 06/12/2015
 *      Author: Airton Ribeiro
 */

#ifndef EXTERN_H_
#define EXTERN_H_

#ifdef __cplusplus
extern "C" {
#endif

int iniciar(char* porta);
int ler();
int getTemperatura();
int getUmidade();
int getLdr();
int getSom();
int getRfid();
int finalizar();

#ifdef __cplusplus
}
#endif

#endif /* EXTERN_H_ */
