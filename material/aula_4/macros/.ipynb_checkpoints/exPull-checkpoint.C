#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLine.h"
#include "TMath.h"
#include "TLatex.h"

#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooHist.h"
#include "RooFitResult.h"
#include "RooFit.h"

void exPull() {

    RooRealVar x("x", "x", -10, 10);

    RooRealVar mean("mean", "mean", 0, -10, 10);
    RooRealVar sigma("sigma", "sigma", 1, 0.1, 10);
    RooGaussian gauss("gauss", "Gaussian PDF", x, mean, sigma);

    RooDataSet* data = gauss.generate(x, 1000);

    // Create a frame to plot on
    RooPlot* frame = x.frame();

    // Plot the data
    data->plotOn(frame);

    // Fit the Gaussian to the data
    RooFitResult* fitResult = gauss.fitTo(*data, RooFit::Save());

    gauss.plotOn(frame);

    // Create pull histogram from fit and data
    RooHist* pullHist = frame->pullHist();

    // Convert RooHist to TH1F for further manipulation
    int nBins = pullHist->GetN();
    TH1F* pullHistTH1 = new TH1F("pullHistTH1", "Pull Distribution", 50, -10, 10);  

    // Fill TH1F with the pull values (Y-axis values from the pullHist)
    for (int i = 0; i < nBins; ++i) {
        double x, y;
        pullHist->GetPoint(i, x, y);  // x is ignored, we want y (the pull)
        double ey = pullHist->GetErrorY(i);  // Get the error on Y
        if (ey > 0) {
            double pullValue = y / ey;  // Normalize the pull by its error
            pullHistTH1->Fill(pullValue);
        }
    }

    // Calculate mean and sigma of the pull values
    double pullMean = pullHistTH1->GetMean();
    double pullMeanError = pullHistTH1->GetMeanError(); 
    double pullSigma = pullHistTH1->GetRMS();
    double pullSigmaError = pullHistTH1->GetRMSError();  

    // Plot the data and the fit on a canvas
    TCanvas* c = new TCanvas("fit_canvas", "Fit and Pull Distribution", 800, 800);
    c->Divide(1, 2);

    // Top pad: data and fit
    c->cd(1);
    gPad->SetPad(0, 0.3, 1, 1);
    gPad->SetBottomMargin(0.02);  // Adjust margins for proper alignment
    frame->GetXaxis()->SetLabelSize(0);
    frame->Draw();
    gauss.paramOn(frame);

    // Bottom pad: pull distribution
    c->cd(2);
    gPad->SetPad(0, 0, 1, 0.3);
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.2);  // Leave space for axis labels
    pullHistTH1->GetYaxis()->SetTitle("Pull");
    pullHistTH1->GetYaxis()->SetTitleOffset(1.4);
    pullHistTH1->Draw();

    // Optionally draw a horizontal line at 0 for visual reference
    TLine* line = new TLine(-10, 0, 10, 0);
    line->SetLineStyle(2);  // Dashed line
    line->Draw("same");

    // Print mean and sigma on the pull distribution plot
    TLatex latex;
    latex.SetNDC();  // Use normalized coordinates (0,1)
    latex.SetTextSize(0.07);
    latex.DrawLatex(0.2, 0.8, Form("Mean = %.3f #pm %.3f", pullMean, pullMeanError));
    latex.DrawLatex(0.2, 0.7, Form("Sigma = %.3f #pm %.3f", pullSigma, pullSigmaError));
   

    // Save the canvas
    c->SaveAs("pull_distribution_with_stats.png");

    // Clean up
    delete c;
    delete data;
    delete fitResult;
    delete pullHistTH1;
}


