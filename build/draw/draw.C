#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>



void draw() {
    TFile *f = TFile::Open("TotalEnergyScan.root");
    // TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
    // TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
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



    h2->SetLineWidth(2);
    h2->GetXaxis()->CenterTitle();
    h2->GetXaxis()->SetTitle("Energy [MeV]");
    h2->GetYaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("Normalized Counts");
    h2->GetYaxis()->SetTitleOffset(1.2);
    h2->Scale(1.0 / 10000.0);
    h2->Draw("HIST");
   

    TCanvas *c1 = new TCanvas("c1", "Detector 1 Energy Scan", 700, 500);
    h1->Draw("HIST");
    auto legend1 = new TLegend(0.68, 0.8, 0.89, 0.88);
    legend1->SetHeader("Angle #theta = 0 degrees"); // option "C" allows to center the header
    legend1->AddEntry(h1,"Front Detector");
    legend1->Draw();

    TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    h2->Draw("HIST");
    auto legend2 = new TLegend(0.68, 0.2, 0.89, 0.28);
    legend2->SetHeader("Angle #theta = 0 degrees"); // option "C" allows to center the header
    legend2->AddEntry(h2,"Rear Detector");
    legend2->Draw();

}



