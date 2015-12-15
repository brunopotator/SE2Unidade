package br.edu.ifba.se.incubadora;

import java.util.ArrayList;
import java.util.List;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;

import org.primefaces.model.chart.Axis;
import org.primefaces.model.chart.AxisType;
import org.primefaces.model.chart.CategoryAxis;
import org.primefaces.model.chart.LineChartModel;
import org.primefaces.model.chart.LineChartSeries;
import org.primefaces.model.chart.MeterGaugeChartModel;

import br.edu.ifba.se.incubadora.conector.SingleConector;

@ManagedBean(name = "monitor")
public class Monitor {

	// vamos configurar o medidor para identificar o valor final, inicial,
	// temperatura do componente da interface
	private MeterGaugeChartModel modeloMedidorTemperatura;
	private MeterGaugeChartModel modeloMedidorUmidade;
	private MeterGaugeChartModel modeloMedidorLuminosidade;
	private LineChartModel modeloMedidorSom;
	
	private LineChartSeries series = new LineChartSeries();
	private static int [] valSeries = new int[50];

	public MeterGaugeChartModel getModeloMedidorTemperatura() {
		return modeloMedidorTemperatura;
	}

	public MeterGaugeChartModel getModeloMedidorUmidade() {
		return modeloMedidorUmidade;
	}

	public MeterGaugeChartModel getModeloMedidorLuminosidade() {
		return modeloMedidorLuminosidade;
	}

	public LineChartModel getModeloMedidorSom() {
		return modeloMedidorSom;
	}
	
	@PostConstruct
	public void iniciar() {
		configurarMedidores();

	}

	private void configurarMedidores() {
		modeloMedidorTemperatura = criarModeloTemperatura();
		modeloMedidorTemperatura.setTitle("Temperatura");
		modeloMedidorTemperatura.setGaugeLabel("Celsius");
		modeloMedidorTemperatura.setSeriesColors("FF0000,FFFF00,00FF00,FFFF00,FF0000");
		
		modeloMedidorUmidade = criarModeloUmidade();
		modeloMedidorUmidade.setTitle("Umidade");
		modeloMedidorUmidade.setGaugeLabel("Percentual");
		modeloMedidorUmidade.setSeriesColors("FF0000,FFFF00,00FF00,FFFF00,FF0000");

		modeloMedidorLuminosidade = criarModeloLuminosidade();
		modeloMedidorLuminosidade.setTitle("Luminosidade");
		modeloMedidorLuminosidade.setGaugeLabel("Valor");
		modeloMedidorLuminosidade.setSeriesColors("FFFF00,008B00,00FF00,008B00,FFFF00");

		modeloMedidorSom = criarModeloSom();
		modeloMedidorSom.setTitle("Presença Sonora");
		Axis yAxis = modeloMedidorSom.getAxis(AxisType.Y);
		modeloMedidorSom.setShowPointLabels(false);
		modeloMedidorSom.getAxes().put(AxisType.X, new CategoryAxis("Leitura"));
		yAxis.setLabel("Valor");
		yAxis.setMin(0);
		yAxis.setMax(255);

	}

	private MeterGaugeChartModel criarModeloTemperatura() {
		List<Number> marcadores = new ArrayList<Number>();
		marcadores.add(30);
		marcadores.add(37);
		marcadores.add(38);
		marcadores.add(40);
		return new MeterGaugeChartModel(0, marcadores);
	}

	private MeterGaugeChartModel criarModeloUmidade() {
		List<Number> marcadores = new ArrayList<Number>();
		marcadores.add(25);
		marcadores.add(50);
		marcadores.add(75);
		marcadores.add(100);

		return new MeterGaugeChartModel(0, marcadores);
	}

	private MeterGaugeChartModel criarModeloLuminosidade() {
		List<Number> marcadores = new ArrayList<Number>();
		marcadores.add(51);
		marcadores.add(102);
		marcadores.add(153);
		marcadores.add(204);
		marcadores.add(255);

		return new MeterGaugeChartModel(0, marcadores);
	}

	private LineChartModel criarModeloSom() {
		LineChartModel modelo = new LineChartModel();
		for (int i=0; i<49; i++) {
			series.set(i+1, 0);
		}

		modelo.addSeries(series);

		return modelo;

	}

	private void atualizarModeloSom(int som) {
		for (int i=0; i<49; i++) {
			valSeries[i] = valSeries[i+1];
			series.set(i+1, valSeries[i]);
		}
		valSeries[49] = som;
		series.set(50, som);

		modeloMedidorSom.addSeries(series);

	}

	public void lerSensores() {
		Informacao info = SingleConector.getInformacao();

		int temperatura = info.getTemperatura();
		int umidade = info.getUmidade();
		int luminosidade = info.getLuminosidade();
		int som = info.getSom();

		System.out.println("temperatura " + info.getTemperatura());
		System.out.println("umidade " + info.getUmidade());
		System.out.println("luminosidade " + info.getLuminosidade());
		System.out.println("som " + info.getSom());

		// envia para o monitor .xhtml
		modeloMedidorTemperatura.setValue(temperatura);
		modeloMedidorUmidade.setValue(umidade);
		modeloMedidorLuminosidade.setValue(luminosidade);
		atualizarModeloSom(som);

	}
}
