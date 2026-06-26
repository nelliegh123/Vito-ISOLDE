#include "eventAction.hh"
#include "sensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

//At end of event, it should fetch data from sensitiveDetector and fill histograms

eventAction::eventAction() 
{}

eventAction::~eventAction()
{}

void eventAction::BeginOfEventAction(const G4Event*)
{}

void eventAction::EndOfEventAction(const G4Event* event) {
    auto sdManager = G4SDManager::GetSDMpointer();

    auto det1 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector1SD"));
    auto det2 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector2SD"));

    if (!det1 || !det2) return;

    G4double energy = 0.0;
    if (event->GetNumberOfPrimaryVertex() > 0) {
        energy = event->GetPrimaryVertex(0)->GetPrimary(0)->GetKineticEnergy();
    }

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, energy);
    analysisManager->FillNtupleIColumn(1, det1->GetCount());
    analysisManager->FillNtupleIColumn(2, det2->GetCount());
    analysisManager->AddNtupleRow(); 

}