#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(const G4String& name);
    ~MySensitiveDetector() override = default;

    void Initialize(G4HCofThisEvent*) override;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

    void EndOfEvent(G4HCofThisEvent*) override;

private:
    int fCounter = 0;
};

#endif