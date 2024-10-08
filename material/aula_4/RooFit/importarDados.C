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
    TFile *file = TFile::Open(" /opendata/example_data.root");


    TTree *myTree = (TTree *)file->Get("nt");

    // Definir a variável observável com o nome 'mass'
    RooRealVar x("mass", "mass", 0.0 , 2.0);  

    // Importar dados unbinned com RooDataSet
    RooDataSet dataUnbinned("dataUnbinned", "Unbinned data", RooArgSet(x), RooFit::Import(*myTree));
    std::cout << "Dados importados: " << std::endl;
    dataUnbinned.Print("v");

    TCanvas *c = new TCanvas("c", "Imported Data", 800, 600);

    RooPlot* frame = x.frame();

    dataUnbinned.plotOn(frame, RooFit::MarkerColor(kBlue), RooFit::Name("Unbinned"));

    frame->Draw();

    c->Draw();

    c->SaveAs("ImportarDadosUnbinned.png");

    file->Close();
}

