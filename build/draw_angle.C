#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_angle() {
    TFile *f = TFile::Open("TotalAngleScan.root");
    if (!f || f->IsZombie()) {
        printf("Error: Could not open TotalAngleScan.root\n");
        return;
    }

    TH1D *h1 = (TH1D*)f->Get("Detector1_Scan_Angle");
    TH1D *h2 = (TH1D*)f->Get("Detector2_Scan_Angle");

    if (!h1 || !h2) {
        printf("Error: Could not find the histograms inside the file!\n");
        return;
    }

 
    h1->SetLineColor(kBlack);
    h1->SetLineWidth(2);
    h1->GetXaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("Energy [MeV]");
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("Counts");

 
    h2->SetLineColor(kBlack);
    h2->SetLineWidth(2);
    h2->GetXaxis()->CenterTitle();
    h2->GetXaxis()->SetTitle("Energy [MeV]");
    h2->GetYaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("Counts");


   

    TCanvas *c1 = new TCanvas("c1", "Detector 1 Energy Scan", 700, 500);
    h1->Draw("HIST");

    TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    h2->Draw("HIST");
}