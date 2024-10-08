// Inclua os cabeçalhos necessários
#include "TCanvas.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooFit.h"

void plotGaussian() {

    // Definir as variáveis x, mean e width com seus intervalos 
    RooRealVar x("x", "x", 0, -10, 10);

    RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);

    RooRealVar width("width", "Width of Gaussian", 3, 0.1, 10);

    // Criar a função gaussiana
    RooGaussian g("g", "Gaussian", x, mean, width);

    // Criar um objeto RooPlot que serve como um "frame" para o gráfico
    RooPlot* frame = x.frame();

    // Plotar a PDF no frame (a função gaussiana g)
    g.plotOn(frame);

    TCanvas* c1 = new TCanvas("c1", "Gaussian Plot", 800, 600);
    gStyle->SetOptStat(1111);  // Ativar a caixa de estatísticas com informações completas
    
    frame->Draw();

    c1->Draw();

    c1->SaveAs("gauss.pdf");
}
