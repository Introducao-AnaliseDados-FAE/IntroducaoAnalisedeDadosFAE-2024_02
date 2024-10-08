#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"

void chi2_example() {
   
    RooRealVar x("x", "x", -10, 10);
    
    RooRealVar mean("mean", "mean of gaussian", 0, -10, 10);
    RooRealVar sigma("sigma", "width of gaussian", 3, 0.1, 10);
    
    RooGaussian gauss("gauss", "gaussian PDF", x, mean, sigma);
    
    RooDataSet* data = gauss.generate(x, 1000);  
    
    RooPlot* frame = x.frame();
    
    data->plotOn(frame);
    
    gauss.fitTo(*data);
    //RooFitResult* fit_result = gauss.fitTo(*data, ROOT::RooFit::Save());

    
    gauss.plotOn(frame);
    
    // Calcular o chi2
    //nFloatParam = fit_result.floatParsFinal().getSize()

    int nFloatParam = 2;  // número de parâmetros flutuantes (mean e sigma)
    double chi2 = frame->chiSquare(nFloatParam);
    
    TCanvas* c = new TCanvas("c", "Chi2 Example", 900, 700);
    frame->Draw();
    
    // Adicionar um TLegend para exibir informações dos parâmetros e chi2
    TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetTextSize(0.03);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry((TObject*)0, Form("Mean = %.3f #pm %.3f", mean.getVal(), mean.getError()), "");
    leg->AddEntry((TObject*)0, Form("Sigma = %.3f #pm %.3f", sigma.getVal(), sigma.getError()), "");
    leg->AddEntry((TObject*)0, Form("#chi^{2}/ndf = %.2f", chi2), "");
    leg->Draw();


    c->Update();
    
    c->Draw();
    c->SaveAs("chi2ex.png");
    // Mostrar os resultados do ajuste no terminal
    //fit_result->Print();
}

