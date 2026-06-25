#include "sensitiveDetector.hh"

#include <TFile.h>

MySensitiveDetector::MySensitiveDetector(const G4String& name):G4VSensitiveDetector(name)
{
    detector1 = std::unique_ptr<TFile>(TFile::Open("detector1_Hits.root", "RECREATE"));
    // auto detector2 = std::unique_ptr<TFile>(TFile::Open("detector2_Hits.root", "RECREATE"));
}

void MySensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fCounter = 0;
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    if (step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary) 
        return false;

    fCounter++;
    std::cout << fCounter << std::endl;
    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    detector1->WriteObject(&fCounter, "hit1");
    G4cout << G4endl << "-------->Sensitive Detector " << SensitiveDetectorName << " counted " << fCounter << G4endl;
}




// detector1->Close();