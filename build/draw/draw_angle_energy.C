TH2D *h2 = new TH2D("h2", "Angle vs Energy",
                     46, 0, 180,   // x-axis = angle
                     19, 0, 10);


#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_angle_energy(double numberOfParticles=1.0) {
    gStyle->SetOptStat(0);

    TFile *f = TFile::Open("TotalAngleScan.root");
    if (!f || f->IsZombie()) {
        printf("Error: Could not open TotalAngleScan.root\n");
        return;
    }

    TH1D *h1 = (TH1D*)f->Get("Detector_E_vs_Angle_1");
    TH1D *h2 = (TH1D*)f->Get("Detector_E_vs_Angle_2");

    if (!h1 || !h2) {
        printf("Error: Could not find the histograms inside the file!\n");
        return;
    }
 

    TCanvas *c1 = new TCanvas("c1", "Detector 1 Energy Scan", 700, 500);
    h1->GetXaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("Angle [deg]");
    h1->GetXaxis()->SetTitleOffset(1.5);
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("Energy [MeV]");
    h1->GetYaxis()->SetTitleOffset(1.5);
    h1->Scale(1.0 / numberOfParticles);
    h1->SetBarWidth(1.0);

    h1->Draw("LEGO2");

    TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    h2->GetXaxis()->CenterTitle();
    h2->GetXaxis()->SetTitle("Angle [deg]");
    h2->GetXaxis()->SetTitleOffset(1.5);
    h2->GetYaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("Energy [MeV]");
    h2->GetYaxis()->SetTitleOffset(1.5);
    h2->SetBarWidth(1.0);
    h2->Scale(1.0 / numberOfParticles);
    h2->Draw("LEGO2");
    gPad->SetPhi(gPad->GetPhi() + 290);   // rotate view by 90° around Z
    c2->Modified();
    c2->Update();

        
    // c1->SaveAs("Plots/AngleScanenergy.pdf");
    // c2->SaveAs("Plots/AngleScanEnergyD2.pdf");

    c1->SaveAs("Plots/AngleScanEnergyD1_100.pdf");
    c2->SaveAs("Plots/AngleScanEnergyD2_100.pdf");
    


}