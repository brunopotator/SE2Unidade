/*
 * Comunicacao.cpp
 *
 *  Created on: 06/12/2015
 *      Author: Airton Ribeiro
 */
#include "Comunicacao.h"



#ifdef __linux__
#include<stdio.h>
#include<fcntl.h>
#include <errno.h>
#endif

Comunicacao::Comunicacao(char* porta){
	hPorta=0;
	this->porta = porta;
}


int Comunicacao::iniciar(){
	int resultado = EXIT_SUCCESS;

#if _WIN32 || _WIN64
	hPorta = CreateFile(porta, GENERIC_READ | GENERIC_WRITE, // cria um arquivo para acessar
			0, NULL, OPEN_EXISTING, 0, NULL);
	//caso nao der certo pega o erro do SO
	if (hPorta == INVALID_HANDLE_VALUE){ //verifica se conseguiu pegar o identificador
		//pega o erro se der problema
		resultado = GetLastError();
	} else {
		//configuracao da porta
		DCB dcb; //estrutura de dados do Windows p/ acesso a porta serial
		memset(&dcb, 0, sizeof(dcb)); //zerar a dcb
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = CBR_9600; //definindo a velocidade de leitura da serial
		dcb.Parity = NOPARITY; // vao verifica o bit de paridade
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
		// se eu nao conseguir configurar a porta
		if (!SetCommState(hPorta, &dcb)){
			resultado  = GetLastError();
		}
	}
#endif
//diretivas para Linux

#ifdef __linux__
	hPorta  = open(porta, O_RDWR | O_NOCTTY | O_NDELAY);
	if (hPorta == -1){ // nao conseguiu pegar a porta retorna que falhou
		resultado = EXIT_FAILURE;
	} else { //configuraçao que precisa
		fcntl(hPorta, F_SETFL, 0);
	}

#endif

	Sleep(2000); //esperar 2 segundos

	return resultado;
}


int Comunicacao::ler(char* buffer,                //quantidade de bits que vc quer nem sempre vem tem que verificar
		long unsigned int bytesParaLer){
	int resultado  = EXIT_FAILURE;

	long unsigned int bytesLidos=0; // bytes que ele conseguiu ler
#if _WIN32 | _WIN64
	ReadFile(hPorta, buffer, bytesParaLer, &bytesLidos, NULL); //funcao para ler do windows
#endif

#ifdef __linux__
	bytesLidos = read(hPorta, (void*)buffer, bytesParaLer); // pode ser que nao leia vc verificar quantos foram lidos
#endif

	if (bytesLidos  == bytesParaLer){
		resultado = EXIT_SUCCESS;
	}

	return resultado;

}

//metodo para fechar a porta
int Comunicacao::finalizar(){
//fechar porta no Windows
#if _WIN32 || _WIN64
	CloseHandle(hPorta);
#endif

//fechar porta no Linux
#ifdef _linux_
 close(hPorta)
#endif
	return EXIT_SUCCESS;
}
