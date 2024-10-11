#include "TCanvas.h"
#include "RooPlot.h"
#include "RooHist.h"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataSet.h"
#include "TROOT.h"
#include "TStyle.h"

void testePull() {

    // Definir as variáveis x, mean e width com seus intervalos 
    RooRealVar x("x", "x", 0, -10, 10);
    RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);
    RooRealVar width("width", "Width of Gaussian", 3, 0.1, 10);

    // Criar a função gaussiana
    RooGaussian gauss("gauss", "Gaussian", x, mean, width);

    
    RooDataSet* data = gauss.generate(x, 1000);  

    
    RooPlot* xframe = x.frame();
    data->plotOn(xframe);         
    gauss.plotOn(xframe);         
   
    RooFitResult* result = gauss.fitTo(*data, RooFit::Save());
    //gauss.paramOn(xframe, RooFit::Layout(0.6, 0.9, 0.85));
    gauss.paramOn(xframe, RooFit::Layout(0.5, 0.9, 0.85), RooFit::ShowConstants(kTRUE));

    
    // Criar o histograma de pulls
    RooHist* hpull = xframe->pullHist();

    // Criar um novo frame para o histograma de pulls
    RooPlot* frame2 = x.frame();
    frame2->SetTitle("Pull Distribution");
    frame2->addPlotable(hpull, "P");

    // Criar e desenhar o canvas
    TCanvas* c1 = new TCanvas("c1", "Gaussian Plot", 900, 700);
    c1->Divide(2);
    c1->cd(1);
    gPad->SetLeftMargin(0.15);
    xframe->GetYaxis()->SetTitleOffset(1.6);
    xframe->Draw();

    // Desenhar o histograma de pulls no segundo painel
    c1->cd(2);
    gPad->SetLeftMargin(0.15);
    frame2->GetYaxis()->SetTitleOffset(1.6);
    frame2->GetYaxis()->SetTitle("Pull");
    frame2->Draw();

    // Extrair os valores de Y do histograma de pulls
    std::vector<double> yValues;
    for (int i = 0; i < hpull->GetN(); i++) {
        double x, y;
        hpull->GetPoint(i, x, y);  // Extrai as coordenadas (x, y) dos pontos
        yValues.push_back(y);      // Armazena o valor de y
    }

    // Calcular a média e o sigma (desvio padrão) dos valores de y
    double meanY = TMath::Mean(yValues.begin(), yValues.end());
    double sigmaY = TMath::RMS(yValues.begin(), yValues.end());  
    // Obter o número de elementos
     int N = yValues.size();

    // Calcular os erros na média e no sigma
    double errorMeanY = sigmaY / sqrt(N);        // Erro na média
    double errorSigmaY = sigmaY / sqrt(2 * N);   // Erro no sigma

    // Exibir os resultados
    std::cout << "Mean: " << meanY << " ± " << errorMeanY << std::endl;
    std::cout << "Sigma: " << sigmaY << " ± " << errorSigmaY << std::endl;


    // Imprimir os valores calculados
    //std::cout << "Média do Y da distribuição de pulls: " << meanY << std::endl;
    //std::cout << "Sigma (desvio padrão) do Y da distribuição de pulls: " << sigmaY << std::endl;

    // Adicionar o texto da média e sigma no gráfico de pulls
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.04);
    latex.DrawLatex(0.2, 0.8, Form("Mean = %.3f #pm %.3f", meanY, errorMeanY));
    latex.DrawLatex(0.2, 0.7, Form("Sigma = %.3f #pm %.3f", sigmaY, errorSigmaY));


    c1->Draw();

    c1->SaveAs("gaussComPull.png");

    // Exibir os resultados do ajuste
    result->Print();
}

