#define USE_GPS
#include "primaryGeneratorAction.hh"
#include "G4GenericMessenger.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SingleParticleSource.hh"


MyPrimaryGenerator::MyPrimaryGenerator() 
{
    fScanMode = false;
    fNumParticlesPerPoint = 10;
    fNAngleSteps = 46;
    fNEnergySteps = 19;
    fThetaMin = 0.0;
    fThetaMax = 180.0;
    fEnergyMin = 1.0;
    fEnergyMax = 10.0;

    fMessenger = new G4GenericMessenger(this, "/mygen/", "Primary generator scan control");
    fMessenger->DeclareProperty("scanMode", fScanMode, "Enable/disable grid scan mode");
    fMessenger->DeclareProperty("numParticlesPerPoint", fNumParticlesPerPoint, "Number of particles per grid point");
    fMessenger->DeclareProperty("nAngleSteps", fNAngleSteps, "Number of angle steps");
    fMessenger->DeclareProperty("nEnergySteps", fNEnergySteps, "Number of energy steps");
    fMessenger->DeclareProperty("thetaMin", fThetaMin, "Min angle in degrees");
    fMessenger->DeclareProperty("thetaMax", fThetaMax, "Max angle in degrees");
    fMessenger->DeclareProperty("energyMin", fEnergyMin, "Min energy in MeV");
    fMessenger->DeclareProperty("energyMax", fEnergyMax, "Max energy in MeV");

    #ifdef USE_GPS
        fGPS = new G4GeneralParticleSource();   

    #else
        fParticleGun = new G4ParticleGun(1);
            
        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle("geantino");

        G4ThreeVector pos(0., 0., 0.5);
        G4ThreeVector mom(0., 0., 1.);

        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom);
        fParticleGun->SetParticleMomentum(0.*GeV);
        fParticleGun->SetParticleDefinition(particle);
    #endif
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fMessenger;
    #ifdef USE_GPS
        delete fGPS;
    
    #else
        delete fParticleGun;
    #endif
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    #ifdef USE_GPS 
        if (fScanMode) {
            G4int eventID = anEvent->GetEventID();
            G4int pointID = eventID / fNumParticlesPerPoint;
            G4int angleIdx = pointID / fNEnergySteps;
            G4int energyIdx = pointID % fNEnergySteps;
            
            G4double theta_deg = fThetaMin;
            if (fNAngleSteps > 1) {
                theta_deg += angleIdx * (fThetaMax - fThetaMin) / (fNAngleSteps - 1);
            }
            G4double energy_MeV = fEnergyMin;
            if (fNEnergySteps > 1) {
                energy_MeV += energyIdx * (fEnergyMax - fEnergyMin) / (fNEnergySteps - 1);
            }

            fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy_MeV * MeV);

            G4double theta_rad = theta_deg * CLHEP::deg;
            G4ThreeVector dir(0.0, std::sin(theta_rad), std::cos(theta_rad));
            fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(dir);
        }

        fGPS->GeneratePrimaryVertex(anEvent);
        
    #else
        G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

        if(particle == G4Geantino::Geantino())
        {
            G4int Z = 27; //Protons in cobalt
            G4int A = 60; //Atomic number 

            G4double charge = 0.*eplus;
            G4double energy = 100.*MeV;

            G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);

            fParticleGun->SetParticleDefinition(ion);
            fParticleGun->SetParticleCharge(charge);

        }

        fParticleGun->GeneratePrimaryVertex(anEvent);
    #endif
}

