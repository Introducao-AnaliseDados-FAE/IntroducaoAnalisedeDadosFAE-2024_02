#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TFile.h"

void exemploWorkspace() {
    // Criar o workspace
    RooWorkspace w("w");

    // Definir uma variável de massa com intervalo de 0 a 2 GeV
    w.factory("mass[0, 2]");

    // Definir parâmetros da Gaussiana: mean e sigma
    w.factory("mean[1.0, 0.5, 1.5]");
    w.factory("sigma[0.1, 0.01, 0.5]");

    // Definir uma Gaussiana com esses parâmetros
    w.factory("Gaussian::gauss(mass, mean, sigma)");

    // Simular um conjunto de dados (para simplificar neste exemplo)
    RooRealVar* mass = w.var("mass");
    RooDataSet* data = w.pdf("gauss")->generate(*mass, 1000); // 1000 eventos simulados
    w.import(*data, "data");

    // Ajustar a Gaussiana aos dados
    w.pdf("gauss")->fitTo(*data);

    // Criar um frame para plotar os dados e o ajuste
    RooPlot* frame = mass->frame();
    data->plotOn(frame);
    w.pdf("gauss")->plotOn(frame);

    // Desenhar o gráfico
    TCanvas* c = new TCanvas("c", "Fit Gaussiano no Workspace", 800, 600);
    frame->Draw();

    // Salvar o workspace em um arquivo ROOT
    TFile f("workspace_gauss.root", "RECREATE");
    w.Write();
    f.Close();

    // Salvar o gráfico como imagem
    c->SaveAs("FitGaussianoWorkspace.png");
}
