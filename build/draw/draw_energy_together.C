#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

#=====================================================================
#somethig is off with normalization 
#=====================================================================

void draw_energy_together() {
    TFile *f = TFile::Open("TotalEnergyScan.root");
    TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
    TH1D *h1 = (TH1D*)f->Get("Detector1_Scan");
    TH1D *h2 = (TH1D*)f->Get("Detector2_Scan");

    h1->SetLineWidth(2);
    h1->GetXaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("Energy [MeV]");
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("Normalized Counts");
    h1->GetYaxis()->SetTitleOffset(1.2);
    h1->Scale(1.0 / 10000.0);
    h1->Draw("HIST");


    Float_t rightmax = 1.1*h2->GetMaximum();
    Float_t scale = gPad->GetUymax()/(rightmax);
    h2->SetLineColor(6);
    h2->Scale(scale);
    h2->SetLineWidth(2);
    h2->SetLineStyle(9);
    h2->Draw("HIST same");
    c1->Update();
    

    TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
    axis->SetLineColor(6);
    axis->SetTextColor(6); 
    axis->Draw();

    auto legend = new TLegend(0.68, 0.75, 0.89, 0.88);
    legend->SetHeader("Angle #theta = 180 degrees"); // option "C" allows to center the header
    legend->AddEntry(h1,"Front Detector");
    legend->AddEntry(h2,"Rear Detector");
    legend->Draw();

}