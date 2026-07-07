#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_angle() {
    gStyle->SetOptStat(0);

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
 
    h1->SetLineColor(kRed);
    h1->SetLineWidth(3);
    h1->GetXaxis()->CenterTitle();
    h1->GetXaxis()->SetTitle("Energy [MeV]");
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("Counts");
    h1->Scale(1.0 / 10000.0);


    h2->SetLineColor(kBlue);
    h2->SetLineWidth(3);
    h2->GetXaxis()->CenterTitle();
    h2->GetXaxis()->SetTitle("Angle [deg]");
    h2->GetYaxis()->CenterTitle();
    h2->GetYaxis()->SetTitle("Counts");
    h2->Scale(1.0 / 10000.0);


    TCanvas *c1 = new TCanvas("c1", "Detector 1 Energy Scan", 700, 500);
    h1->Draw("HIST");

    // TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    h2->Draw("HIST same");



    auto legend = new TLegend(0.68, 0.55, 0.89, 0.68);
    legend->SetHeader("Energy = 10 MeV"); // option "C" allows to center the header
    legend->AddEntry(h1,"Front Detector");
    legend->AddEntry(h2,"Rear Detector");
    legend->Draw();



    // TCanvas *c2 = new TCanvas("c2", "Detector 2 Energy Scan", 700, 500);
    TH1 *hsum = (TH1*)h1->Clone("hsum");
    hsum->Add(h2); 
    // hsum->Divide(h2); 
    hsum->SetLineColor(kBlack);
    hsum->SetLineWidth(3);
    hsum->SetLineStyle(9);
    hsum->Draw("HIST same");
    
    c1->SaveAs("Plots/AngleScan1.pdf");
    
    









    for (int i = 1; i <= hsum->GetNbinsX(); i++) {
            if (hsum->GetBinContent(i) > 1.0) {
            std::cout << "Larger than 1 at " << hsum->GetBinCenter(i) << std::endl;
        }
    }

    for (int i = 1; i <= h1->GetNbinsX(); i++) {
            if (h2->GetBinContent(i) > 1.0) {
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << "PROBLEM!! H1 is larger than 1 at " << h1->GetBinCenter(i) << std::endl;
        }
    }

    for (int i = 1; i <= h2->GetNbinsX(); i++) {
            if (h2->GetBinContent(i) > 1.0) {
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << "PROBLEM!! H2 is larger than 1 at " << h2->GetBinCenter(i) << std::endl;
        }
    }
}