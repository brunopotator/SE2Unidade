package br.edu.ifba.se.incubadora.conector;

import java.util.concurrent.Semaphore;

import br.edu.ifba.se.incubadora.IComunicacaoRF;
import br.edu.ifba.se.incubadora.Informacao;

public class SingleConector {
	private static IComunicacaoRF comRF = null;
	private static final String PORTA = "COM3";
	private static Informacao info;

	private static Semaphore semaforo;

	public static void iniciarComunicacaoRF(String libPath) {
		info = new Informacao();
		comRF = FabricaConectores.getConector(libPath);
		if (comRF.iniciar(PORTA) == 0) {
			System.out.println("Acesso a sensores iniciado com sucesso.");
			dispensarPrimeirasLeituras();
			semaforo = new Semaphore(1, true);
		} else {
			System.out.println("Não foi possível iniciar acesso a sensores.");
		}
	}

	public static void dispensarPrimeirasLeituras() {
		for (int i = 0; i < 10; i++) {
			comRF.ler();
			System.out.println("Dispensando leitura: " + comRF.getLdr() + "/ " + comRF.getTemperatura() + "/ "
					+ comRF.getSom() + "/ " + comRF.getUmidade() + "/ " + comRF.getRfid());

			try {
				Thread.sleep(250);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public static int ler() {
		try {
			semaforo.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		int resultado = comRF.ler();
		if (resultado == 0) {
			info.setTemperatura(comRF.getTemperatura());
			info.setLuminosidade(comRF.getLdr());
			info.setSom(comRF.getSom());
			info.setUmidade(comRF.getUmidade());
		}
		semaforo.release();
		return resultado;
	}

	public static Informacao getInformacao() {
		Informacao info = new Informacao();
		try {
			semaforo.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		info.setTemperatura(comRF.getTemperatura());
		info.setLuminosidade(comRF.getLdr());
		info.setSom(comRF.getSom());
		info.setUmidade(comRF.getUmidade());
		semaforo.release();

		return info;
	}

	public static void finalizar() {
		comRF.finalizar();
	}
}
