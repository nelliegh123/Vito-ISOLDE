#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_many_angles() {
    gStyle->SetOptStat(0);
    std::vector<double> energies = {1, 3, 5, 7, 9}; 
    std::vector<int> colors = {kRed, kBlue, kGreen+2, kMagenta, kOrange+1};

    TCanvas *c1 = new TCanvas("c1", "Detector 1 Angle Scan", 700, 500);
    auto legend1 = new TLegend(0.13, 0.6, 0.26, 0.88);

    for (size_t i = 0; i < energies.size(); i++) {
        TFile *f = TFile::Open(Form("rootFiles/Scan_%gMeV.root", energies[i]));
        TH1D *h = (TH1D*)f->Get("Detector1_Scan_Angle");
        h->SetDirectory(0);   // detach from file so it survives after f->Close()
        h->Scale(1.0/100.0);
        h->SetLineWidth(2);
        h->SetLineColor(colors[i]);
        h->GetXaxis()->SetTitle("Angle [Degrees]");
        h->GetYaxis()->SetTitle("Normalized Counts");
        h->Draw(i == 0 ? "HIST" : "HIST SAME");
        legend1->AddEntry(h, Form("%g MeV", energies[i]));
        f->Close();
    }

    legend1->Draw();
    // c1->SaveAs("Plots/AngleScanOverlay.pdf");

    TCanvas *c2 = new TCanvas("c2", "Detector 2 Angle Scan", 700, 500);
    auto legend2 = new TLegend(0.76, 0.6, 0.89, 0.88);

    for (size_t i = 0; i < energies.size(); i++) {
        TFile *f = TFile::Open(Form("rootFiles/Scan_%gMeV.root", energies[i]));
        TH1D *h2 = (TH1D*)f->Get("Detector2_Scan_Angle");
        h2->SetDirectory(0);   // detach from file so it survives after f->Close()
        h2->Scale(1.0/100.0);
        h2->SetLineWidth(2);
        h2->SetLineColor(colors[i]);
        h2->GetXaxis()->SetTitle("Angle [Degrees]");
        h2->GetYaxis()->SetTitle("Normalized Counts");
        h2->SetMaximum(1.05);
        h2->Draw(i == 0 ? "HIST" : "HIST SAME");
        legend2->AddEntry(h2, Form("%g MeV", energies[i]));
        f->Close();
    }


    legend2->Draw();
    // c2->SaveAs("Plots/AngleScanOverlay2.pdf");

}
