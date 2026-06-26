#include "eventAction.hh"
#include "sensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "primaryGeneratorAction.hh"

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
    //////////////////////////////////
    auto runManager = G4RunManager::GetRunManager();
    auto genAction = static_cast<const MyPrimaryGenerator*>(runManager->GetUserPrimaryGeneratorAction());
    
    G4double angle = 0.0;
    if (genAction) {
        angle = genAction->GetCurrentAngle();
    }


    
    // Fill Histogram (e.g., H1 index 4 is Counts vs Angle)
    // x = angle, weight = detector counts
    analysisManager->FillH1(2, angle, det1->GetCount());
    analysisManager->FillH1(3, angle, det2->GetCount());
    //////////////////////////




    analysisManager->FillH1(0, energy, det1->GetCount());
    analysisManager->FillH1(1, energy, det2->GetCount());
    // analysisManager->FillH1(2, energy, det1->GetCount());
    // analysisManager->FillH1(3, energy, det2->GetCount());

}