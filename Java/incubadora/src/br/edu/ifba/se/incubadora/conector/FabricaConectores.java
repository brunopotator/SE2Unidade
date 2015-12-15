package br.edu.ifba.se.incubadora.conector;

import com.sun.jna.Native;
import com.sun.jna.Platform;

import br.edu.ifba.se.incubadora.IComunicacaoRF;

public class FabricaConectores {
	public static IComunicacaoRF getConector(String libPath) {
		IComunicacaoRF comRF = null;

		System.out.println("libPath = " + libPath);
		if (Platform.isWindows()) {

			comRF = (IComunicacaoRF) Native.loadLibrary(libPath + "\\ComunicacaoRF.dll", IComunicacaoRF.class);
			
		} else if (Platform.isLinux()) {
			comRF = (IComunicacaoRF) Native.loadLibrary(libPath
					+ "\\ComunicacaoRF.so", IComunicacaoRF.class);
		}
		return comRF;
	}
}
