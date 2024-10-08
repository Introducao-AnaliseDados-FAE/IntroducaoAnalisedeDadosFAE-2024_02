// Inclua os cabeçalhos necessários
#include "TCanvas.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooArgSet.h"

void generateGaussianEvents() {
    
    RooRealVar x("x", "x", -10, 10);

    RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);

    RooRealVar width("width", "Width of Gaussian", 2, 0.1, 10);

    RooGaussian gaussian("gaussian", "Gaussian PDF", x, mean, width);

    // Gerar 10.000 eventos a partir da PDF
    RooDataSet* data = gaussian.generate(RooArgSet(x), 10000);

    gaussian.fitTo(*data);
    RooPlot* frame = x.frame();
    data->plotOn(frame);

    // Plotar a função PDF Gaussiana ajustada no mesmo frame
    gaussian.plotOn(frame);

    TCanvas* c1 = new TCanvas("c1", "Gaussian Event Generation", 800, 600);

    frame->Draw();

    c1->Draw();
    c1->SaveAs("gaussianEventos.png");
}
