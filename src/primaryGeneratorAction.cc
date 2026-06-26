#define USE_GPS
#include "primaryGeneratorAction.hh"
#include "G4UIcmdWithADouble.hh"   //


MyPrimaryGenerator::MyPrimaryGenerator() : fCurrentAngle(0.0)//
{
    fAngleCmd = new G4UIcmdWithADouble("/mygen/setAngle", this);//
    fAngleCmd->SetGuidance("Set the current run angle in degrees or radians");//

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
    delete fAngleCmd; //
    #ifdef USE_GPS
        delete fGPS;
    
    #else
        delete fParticleGun;
    #endif
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    #ifdef USE_GPS 
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

//////////////////////
void MyPrimaryGenerator::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fAngleCmd) {
        fCurrentAngle = G4UIcmdWithADouble::GetNewDoubleValue(newValue);
    }
}
/////////////////////