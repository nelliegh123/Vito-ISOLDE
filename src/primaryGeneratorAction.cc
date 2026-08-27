#include "primaryGeneratorAction.hh"
#include "G4GenericMessenger.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SingleParticleSource.hh"
#include "Randomize.hh"


MyPrimaryGenerator::MyPrimaryGenerator() 
{
    fScanMode = false;
    fNumParticlesPerPoint = 1;
    fNAngleSteps = 1;
    fNEnergySteps = 1;
    fThetaMin = 0;
    fThetaMax = 0;
    fEnergyMin = 0;
    fEnergyMax = 0;


    fMessenger = new G4GenericMessenger(this, "/mygen/", "Primary generator scan control");
    fMessenger->DeclareProperty("scanMode", fScanMode, "Enable/disablke grid scan mode");
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
        G4double phi_rad = 2.0 * CLHEP::pi * G4UniformRand();
        G4double sinTheta = std::sin(theta_rad);
        G4double dx = sinTheta * std::cos(phi_rad);
        G4double dy = sinTheta * std::sin(phi_rad);
        G4double dz = std::cos(theta_rad);
        G4ThreeVector dir(dx, dy, dz);

        // Rotate into the tilted disc's frame so theta=0 follows the disc's
        // actual outward normal (45 deg from world z), matching fRot in
        // detectorConstruction.cc, instead of firing along raw world z.
        // dir.rotateX(-45.*CLHEP::deg);

        fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(dir);
    }
    fGPS->GeneratePrimaryVertex(anEvent);
}
