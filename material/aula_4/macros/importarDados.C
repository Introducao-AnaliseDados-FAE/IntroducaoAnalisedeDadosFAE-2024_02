#include "TTree.h"
#include "TH1.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "TCanvas.h"
#include <iostream>

//using namespace RooFit;

void importarDados() {
    gStyle->SetOptFit(1111);  // Mostrar os parâmetros do ajuste, Chi2, etc.
    // Abrir o arquivo ROOT
    TFile *file = TFile::Open("/opendata/example_data.root");
    
    TTree *myTree = (TTree *)file->Get("nt");

    // Definir a variável observável com o nome 'mass'
    RooRealVar x("mass", "mass", 0.0 , 2.0);  

    // Importar dados unbinned com RooDataSet
    RooDataSet dataUnbinned("dataUnbinned", "Unbinned data", RooArgSet(x), RooFit::Import(*myTree));
    std::cout << "Dados importados: " << std::endl;
    dataUnbinned.Print("v");

    // Definir a função gaussiana (sinal)
    RooRealVar mean("mean", "Mean of Gaussian", 1.0, 0.9, 1.1);
    RooRealVar sigma("sigma", "Width of Gaussian", 0.1, 0.01, 0.2);
    RooGaussian gauss("gauss", "Gaussian PDF", x, mean, sigma);
    // Definir os parâmetros da exponencial (fundo)
    RooRealVar expParam("expParam", "Exponential decay", -1.0, -5.0, 0.0);
    RooExponential expo("expo", "Exponential PDF", x, expParam);
    
    // Ajustar a função aos dados
    //gauss.fitTo(dataUnbinned);
    //expo.fitTo(dataUnbinned);
    
    // Combinar os dois modelos
    RooRealVar frac("frac", "Fraction of Gaussian", 0.5, 0.0, 1.0);
    RooAddPdf model("model", "gauss+expo", RooArgList(gauss, expo), RooArgList(frac));
    model.fitTo(dataUnbinned);

    TCanvas *c = new TCanvas("c", "Imported Data", 800, 600);

    RooPlot* frame = x.frame();

    dataUnbinned.plotOn(frame, RooFit::MarkerColor(kBlue), RooFit::Name("Unbinned"));
    //Plotar a Gaussiana+expo ajustada no mesmo frame
    //gauss.plotOn(frame, RooFit::LineColor(kGreen), RooFit::Name("Gaussian Fit"));
    //expo.plotOn(frame, RooFit::LineColor(kMagenta), RooFit::Name("Expo Fit"));
    model.plotOn(frame, RooFit::LineColor(kRed), RooFit::Name("Model Fit"));
    model.plotOn(frame, RooFit::Components("gauss"), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen), RooFit::Name("Gaussian"));
    model.plotOn(frame, RooFit::Components("expo"), RooFit::LineStyle(kDotted), RooFit::LineColor(kMagenta), RooFit::Name("Exponential"));
    frame->Print();
    frame->Draw();
    // Adicionar legenda
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);  // Posição da legenda
    legend->AddEntry(frame->findObject("Unbinned"), "Data", "p");
    legend->AddEntry(frame->findObject("Model Fit"), "Model (Gaussian + Exponential)", "l");
    legend->AddEntry(frame->findObject("Gaussian"), "Gaussian Component", "l");
    legend->AddEntry(frame->findObject("Exponential"), "Exponential Component", "l");
    
    // Garantir que a legenda é desenhada
    legend->SetBorderSize(1);  // Borda visível para verificar a presença da legenda
    legend->Draw();

    // Atualizar o canvas para garantir que a legenda seja mostrada
    c->Update();

    //c->Draw();

    c->SaveAs("ImportarDadosFitModel.png");

    file->Close();
}

