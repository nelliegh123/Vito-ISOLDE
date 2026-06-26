#include "sensitiveDetector.hh"
//Responsible for counting hits

MySensitiveDetector::MySensitiveDetector(const G4String& name):G4VSensitiveDetector(name)
{}

void MySensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fCounter = 0;
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    if (step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary) 
        return false;

    fCounter++;
    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}



