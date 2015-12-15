package br.edu.ifba.se.incubadora;

import com.sun.jna.Library;

public interface IComunicacaoRF extends Library {
	
	public int iniciar(String porta);
	
	public int ler();
	
	public int getTemperatura();
	
	public int getUmidade();
	
	public int getLdr();
	
	public int getSom();
	
	public int getRfid();
	
	public int finalizar();
}
