#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include <TFile.h>
#include <set>

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(const G4String& name);
    ~MySensitiveDetector() override = default;

    void Initialize(G4HCofThisEvent*) override;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

    void EndOfEvent(G4HCofThisEvent*) override;

    G4int GetCount() const {return fCounter;}

    std::unique_ptr<TFile> detector1;

private:
    G4int fCounter = 0;
    G4String particleName;
    std::set<G4int> seenTracks;
};

#endif