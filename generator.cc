#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); //The "1" here means that every event 
    //                                     starts from 1 primary particle
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    G4ThreeVector pos(0., 0., -0.99*m);
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(100.*GeV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}