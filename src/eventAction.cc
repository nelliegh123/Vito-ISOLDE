#include "eventAction.hh"
#include "sensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

eventAction::eventAction() 
{}

eventAction::~eventAction()
{}

void eventAction::BeginOfEventAction(const G4Event*)
{
    fStartTime = std::chrono::steady_clock::now();
}

void eventAction::EndOfEventAction(const G4Event* event) {
    auto sdManager = G4SDManager::GetSDMpointer();

    auto det1 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector1SD"));
    auto det2 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector2SD"));
    if (!det1 || !det2) return;

    G4double energy = 0.0;

    G4double angle = 0.0;
    if (event->GetNumberOfPrimaryVertex() > 0) {
        auto primaryParticle = event->GetPrimaryVertex(0)->GetPrimary(0);
        if (primaryParticle) {
            energy = primaryParticle->GetKineticEnergy();
            G4ThreeVector dir = primaryParticle->GetMomentumDirection();
            angle = dir.theta() * 180.0 / 3.14159265358979323846;
        }
    }
    

    auto analysisManager = G4AnalysisManager::Instance();

    //-------------Filling TH2D hist for visualization----------
    analysisManager->FillH2(0, angle, energy, det1->GetCount());
    analysisManager->FillH2(1, angle, energy, det2->GetCount());

    
    //------------------Filling TTree---------------------
    G4int nHit1 = det1->GetCount();
    G4int nHit2 = det2->GetCount();

    if (nHit1 > 0 && nHit2 > 0) {
    // Remove backscattering by killing second event
        G4double t1 = det1->GetHitTime();
        G4double t2 = det2->GetHitTime();
        //here I could sum all backscatter for statistics
        if (t1 < t2) { nHit2 = 0; }
        else         { nHit1 = 0; }
    }

    if (nHit1 > 0) {
        analysisManager->FillNtupleDColumn(0, energy);
        analysisManager->FillNtupleDColumn(1, angle);
        analysisManager->FillNtupleIColumn(2, 0);
        analysisManager->AddNtupleRow();
    }
    
    if (nHit2 > 0) {
        analysisManager->FillNtupleDColumn(0, energy);
        analysisManager->FillNtupleDColumn(1, angle);
        analysisManager->FillNtupleIColumn(2, 1);
        analysisManager->AddNtupleRow();
    }


    // //////////////COULD PROBABLY REMOVE////////////////
    // analysisManager->FillH1(0, energy, det1->GetCount());
    // analysisManager->FillH1(1, energy, det2->GetCount());
    // analysisManager->FillH1(2, angle, det1->GetCount());
    // analysisManager->FillH1(3, angle, det2->GetCount());
    ////////////////////////////////////////////////////

    
}