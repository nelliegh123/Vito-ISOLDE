#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4UImessenger.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"

class G4UIcmdWithADouble;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction, public G4UImessenger
{
public: 
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

    virtual void SetNewValue(G4UIcommand* command, G4String newValue) override; //

    G4double GetCurrentAngle() const { return fCurrentAngle; } //

private: 
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fGPS;

    G4double fCurrentAngle; //
    G4UIcmdWithADouble* fAngleCmd;//
};

#endif