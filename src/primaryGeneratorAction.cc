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

    fGPS = new G4GeneralParticleSource();   
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fMessenger;
    delete fGPS;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
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
}

