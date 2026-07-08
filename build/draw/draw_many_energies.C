#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_many_energies() {
    std::vector<double> angles = {0, 40, 80, 120, 160}; 
    std::vector<int> colors = {kRed, kBlue, kGreen+2, kMagenta, kOrange+1};

    TCanvas *c1 = new TCanvas("c1", "Detector 1 Energy Scan", 700, 500);
    auto legend1 = new TLegend(0.65, 0.6, 0.89, 0.88);

    for (size_t i = 0; i < angles.size(); i++) {
        TFile *f = TFile::Open(Form("rootFiles/Scan_%gDegrees.root", angles[i]));
        TH1D *h = (TH1D*)f->Get("Detector1_Scan");
        h->SetDirectory(0);   // detach from file so it survives after f->Close()
        h->Scale(1.0/1000.0);
        h->SetLineWidth(2);
        h->SetLineColor(colors[i]);
        h->GetXaxis()->SetTitle("Energy [MeV]");
        h->GetYaxis()->SetTitle("Normalized Counts");
        h->Draw(i == 0 ? "HIST" : "HIST SAME");
        legend1->AddEntry(h, Form("%g Degrees", angles[i]));
        f->Close();
    }

    TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    auto legend2 = new TLegend(0.65, 0.6, 0.89, 0.88);

    for (size_t i = 0; i < angles.size(); i++) {
        TFile *f = TFile::Open(Form("rootFiles/Scan_%gDegrees.root", angles[i]));
        TH1D *h2 = (TH1D*)f->Get("Detector2_Scan");
        h2->SetDirectory(0);   // detach from file so it survives after f->Close()
        h2->Scale(1.0/1000.0);
        h2->SetLineWidth(2);
        h2->SetLineColor(colors[i]);
        h2->GetXaxis()->SetTitle("Energy [MeV]");
        h2->GetYaxis()->SetTitle("Normalized Counts");
        h2->Draw(i == 0 ? "HIST" : "HIST SAME");
        legend2->AddEntry(h2, Form("%g Degrees", angles[i]));
        f->Close();
    }

    legend1->Draw();
    c1->SaveAs("Plots/EnergyScanOverlay.pdf");

    legend2->Draw();
    c2->SaveAs("Plots/EnergyScanOverlay2.pdf");

}