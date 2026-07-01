#include "sensitiveDetector.hh"

#include "G4Electron.hh"
//Responsible for counting hits

MySensitiveDetector::MySensitiveDetector(const G4String& name):G4VSensitiveDetector(name)
{}

void MySensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fCounter = 0;
    seenTracks.clear();
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4Track *track = step->GetTrack();

    if (track->GetParentID() != 0)
        return false;

    if (step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary) 
        return false;

    G4int id = track->GetTrackID();

    if (seenTracks.find(id) != seenTracks.end())
       return false;

    seenTracks.insert(id);

    fCounter++;

    
    // G4cout << "==============================" << G4endl;
    // std::cout << particleName << std::endl;
    // G4cout << "==============================" << G4endl;

    return true;

}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}



