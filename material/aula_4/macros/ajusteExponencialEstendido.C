#include "RooRealVar.h"
#include "RooExponential.h"
#include "RooExtendPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TPaveText.h"

void ajusteExponencialEstendido() {
    // 1. Definir a variável x no intervalo de 0 a 10
    RooRealVar x("x", "x", 0, 10);

    // 2. Definir o parâmetro lambda e a função exponencial decrescente
    RooRealVar lambda("lambda", "lambda", 1.0, 0.1, 2.0);
    RooExponential expo("expo", "Exponential PDF", x, lambda);

    // 3. Definir o número total de eventos (nEvents) e criar a PDF estendida
    RooRealVar nEvents("nEvents", "Number of Events", 1500, 0, 10000);
    RooExtendPdf extendedExpo("extendedExpo", "Extended Exponential PDF", expo, nEvents);

    // 4. Gerar 1500 eventos simulados com base na distribuição exponencial
    RooDataSet* data = extendedExpo.generate(x, 1500);

    // 5. Ajustar a função exponencial aos dados (ajuste estendido)
    extendedExpo.fitTo(*data, RooFit::Extended());

    // 6. Criar um frame para a variável x e plotar os dados e o ajuste
    RooPlot* frame = x.frame();
    data->plotOn(frame);                          // Plotar os dados
    extendedExpo.plotOn(frame);                   // Plotar o ajuste

    // 7. Criar um TPaveText para exibir os parâmetros ajustados no gráfico
    TPaveText* paramBox = new TPaveText(0.6, 0.7, 0.9, 0.9, "NDC");
    paramBox->AddText(Form("lambda = %.3f", lambda.getVal()));
    paramBox->AddText(Form("N events = %.1f", nEvents.getVal()));
    paramBox->SetFillColor(0); // Definir cor de fundo transparente
    frame->addObject(paramBox); // Adicionar a caixa ao frame

    // 8. Criar o canvas e desenhar o gráfico
    TCanvas* c = new TCanvas("c", "Ajuste Exponencial Estendido", 800, 600);
    frame->Draw();
    c->SaveAs("ajuste_exponencial_estendido.png");

    // 9. Imprimir os resultados ajustados
    std::cout << "Valor ajustado para lambda: " << lambda.getVal() << std::endl;
    std::cout << "Número total de eventos ajustados: " << nEvents.getVal() << std::endl;
}

