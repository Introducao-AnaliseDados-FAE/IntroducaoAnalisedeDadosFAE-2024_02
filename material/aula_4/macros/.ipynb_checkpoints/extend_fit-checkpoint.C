#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooFit.h"

void extended_fit() {
   
    RooRealVar x("x", "x", -10, 10);

    // Definir os parâmetros da Gaussiana
    RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);
    RooRealVar sigma("sigma", "Width of Gaussian", 2, 0.1, 10);
    RooGaussian gauss("gauss", "Gaussian PDF", x, mean, sigma);

    // Definir o parâmetro de rendimento (número de eventos)
    RooRealVar nEvents("nEvents", "Number of events", 1000, 0, 10000);

    // Criar a PDF estendida
    RooExtendPdf extGauss("extGauss", "Extended Gaussian PDF", gauss, nEvents);

    // Gerar dados simulados a partir da PDF estendida
    RooDataSet* data = extGauss.generate(x, 1200); 

    RooFitResult* fit_result = extGauss.fitTo(*data, RooFit::Save());



    RooPlot* xframe = x.frame();
    data->plotOn(xframe);       
    extGauss.plotOn(xframe);    
    TCanvas* c1 = new TCanvas("c1", "Gaussiana estendida", 900, 700);
    // Exibir o gráfico
   
    extGauss.paramOn(xframe);
    xframe->Draw();
    fit_result->Print("v");
    c1->Draw();
    c1->SaveAs("fitestendidogauss.png");
}

