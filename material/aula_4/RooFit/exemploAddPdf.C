#include "TTree.h"
#include "TH1.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooPolynomial.h"
#include "RooGaussian.h"
#include "RooFit.h"
#include "TCanvas.h"
#include <iostream>

using namespace RooFit;

void exemploAddPdf(){

// observable
RooRealVar x("x", "random variable", 0.0, 1.0);
    
// Gaussian model
RooRealVar mu("mu", "mean parameter", 0.5, 0.0, 1.0);
RooRealVar sigma("sigma", "width parameter", 0.1, 0.0, 0.3);
RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);
    
// Linear function: 1 + slope*x
RooRealVar slope("slope", "slope parameter", -0.3, -10., 10.);
RooPolynomial linear("linear", "Linear function", x, RooArgSet(slope));
    
// add up: Gaussian + linear
RooRealVar fraction("fraction","fraction of Gaussian",0.5,0.,1.);
RooAddPdf model("model","PDF model", RooArgList(gaus,linear), RooArgList(fraction));
    
// generate random data, plot
RooDataSet *dataset = model.generate(x, 10000);
RooFitResult* fit_result = model.fitTo(*dataset, RooFit::Save());
fit_result->Print("v");

TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
RooPlot* frame = x.frame();
dataset->plotOn(frame);
model.plotOn(frame);
model.plotOn(frame,Components(linear),LineStyle(7),LineColor(kRed));
frame->Draw() ;
c1->Draw() ;
c1->SaveAs("exemploSomaPDF.png");
}
