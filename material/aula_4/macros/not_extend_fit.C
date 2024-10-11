#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooFit.h"

void not_extended_fit() {

    // Definir a variável independente (x)
    RooRealVar x("x", "x", -10, 10);

    // Definir os parâmetros da Gaussiana
    RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);
    RooRealVar sigma("sigma", "Width of Gaussian", 2, 0.1, 10);
    RooGaussian gauss("gauss", "Gaussian PDF", x, mean, sigma);

    // Gerar dados simulados a partir da Gaussiana
    RooDataSet* data = gauss.generate(x, 1000); 

    // Ajustar o modelo (Gaussiana) aos dados
    //gauss.fitTo(*data);
    RooFitResult* fit_result = gauss.fitTo(*data, RooFit::Save());

    // Criar um plot para visualizar o ajuste
    RooPlot* xframe = x.frame();
    data->plotOn(xframe);       // Plotar os dados
    gauss.plotOn(xframe);       // Plotar o ajuste
    
    TCanvas* c1 = new TCanvas("c1", "Gaussiana ", 800, 600);

    gauss.paramOn(xframe);

    // Exibir o gráfico
    xframe->Draw();
    fit_result->Print("v");

    c1->Draw();

    c1->SaveAs("nao_estendida_gaus.png");
}

