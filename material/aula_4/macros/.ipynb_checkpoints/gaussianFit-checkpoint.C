#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include <iostream>

using namespace RooFit;

void gaussianFit(){

   RooRealVar x("x","x",0.0,1.0);
   RooRealVar mu("mu","parametro mean",0.5, 0.0, 1.0);
   RooRealVar sigma("sigma", "parametro sigma",0.1, 0.0, 0.3);

    // Definir a PDF Gaussiana
    RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);

    //Gerar um conj. de dados aleatórios
    RooDataSet* data = gaus.generate(x, 10000);

    //Ajustar a PDF aos dados gerados
    RooFitResult* res = gaus.fitTo(*data, Save(true), Minos(true));

    RooPlot* frame = x.frame();
    data->plotOn(frame);          
    gaus.plotOn(frame);           

    TCanvas* c1 = new TCanvas("c1", "Gaussian Fit", 900, 600);
    frame->Draw();                 
    c1->Draw();                    
    c1->SaveAs("gaussian_fit.png"); 

    // Limpar a memória
    delete data;
    delete res;
    delete frame;

}
