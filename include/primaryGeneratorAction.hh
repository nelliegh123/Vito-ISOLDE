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

class G4GenericMessenger;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public: 
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);


private: 
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fGPS;

    G4GenericMessenger *fMessenger;
    G4bool fScanMode;
    G4int fNumParticlesPerPoint;
    G4int fNAngleSteps;
    G4int fNEnergySteps;
    G4double fThetaMin;
    G4double fThetaMax;
    G4double fEnergyMin;
    G4double fEnergyMax;
};

#endif