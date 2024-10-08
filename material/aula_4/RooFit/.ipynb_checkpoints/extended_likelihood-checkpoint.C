#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TCanvas.h"

void extended_likelihood() {

    RooRealVar x("x", "x", -10, 10);

    // Definir o PDF do sinal (Gaussiana)
    RooRealVar mean("mean", "mean of gaussian", 0, -10, 10);
    RooRealVar sigma("sigma", "width of gaussian", 2, 0.1, 5);
    RooGaussian gauss("gauss", "Gaussian Signal PDF", x, mean, sigma);

    // Definir o PDF do fundo (Exponencial)
    RooRealVar tau("tau", "slope of exponential", -0.5, -5.0, 0.0);
    RooExponential expo("expo", "Exponential Background PDF", x, tau);

    // Número esperado de eventos de sinal e fundo
    RooRealVar nsig("nsig", "number of signal events", 500, 0, 1000);
    RooRealVar nbkg("nbkg", "number of background events", 500, 0, 1000);

    // Modelo estendido combinando sinal e fundo
    RooAddPdf model("model", "Signal + Background", RooArgList(gauss, expo), RooArgList(nsig, nbkg));

    // Gerar um conjunto de dados simulados
    RooDataSet* data = model.generate(x, 1000);

    // Ajustar o modelo aos dados (verossimilhança estendida)
    //model.fitTo(*data, RooFit::Extended());
    RooFitResult* fit_result = model.fitTo(*data, RooFit::Save(), RooFit::Extended());
    // Acessar os parâmetros ajustados e seus erros
    fit_result->Print("v");  // Imprime os detalhes do ajuste

    // Acessar os valores ajustados e incertezas
    double nsig_val = nsig.getVal();
    double nsig_err = nsig.getError();
    double nbkg_val = nbkg.getVal();
    double nbkg_err = nbkg.getError();

    std::cout << "Número ajustado de eventos de sinal: " << nsig_val << " ± " << nsig_err << std::endl;
    std::cout << "Número ajustado de eventos de fundo: " << nbkg_val << " ± " << nbkg_err << std::endl;
    
    // Criar um gráfico para a variável x e plotar os dados
    RooPlot* frame = x.frame();
    data->plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, RooFit::Components(expo), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed)); // Fundo
    model.plotOn(frame, RooFit::Components(gauss), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen)); // Sinal

    // Criar um canvas e desenhar o gráfico
    TCanvas* c = new TCanvas("c", "Extended Likelihood Fit", 900, 700);
    frame->Draw();  
    //model.paramOn(frame);
    model.paramOn(frame, RooFit::Layout(0.6, 0.9, 0.9));
    frame->Draw();
    c->Draw();
    c->SaveAs("SinalFundofitestendido.png");
    
}
