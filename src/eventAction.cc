#include "eventAction.hh"
#include "sensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
// #include "primaryGeneratorAction.hh"

//At end of event, it should fetch data from sensitiveDetector and fill histograms

eventAction::eventAction() 
{}

eventAction::~eventAction()
{}

void eventAction::BeginOfEventAction(const G4Event*)
{}

void eventAction::EndOfEventAction(const G4Event* event) {
    auto sdManager = G4SDManager::GetSDMpointer();

    auto det1 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector1SD"));
    auto det2 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("Detector2SD"));

    if (!det1 || !det2) return;

    G4double energy = 0.0;

    //////////////////
    G4double angle = 0.0;
    if (event->GetNumberOfPrimaryVertex() > 0) {
        auto primaryParticle = event->GetPrimaryVertex(0)->GetPrimary(0);
        if (primaryParticle) {
            energy = primaryParticle->GetKineticEnergy();
            G4ThreeVector dir = primaryParticle->GetMomentumDirection();
            angle = dir.theta() * 180.0 / 3.14159265358979323846;
        }
    }
    //////////////////////




    // if (event->GetNumberOfPrimaryVertex() > 0) {
    //     energy = event->GetPrimaryVertex(0)->GetPrimary(0)->GetKineticEnergy();
    // }

    // auto generatorAction = static_cast<const MyPrimaryGenerator*>(
    //     G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    // G4double angle = 0.0;
    // if (generatorAction) {
    //     angle = generatorAction->GetAngle();
    // }

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillH1(0, energy, det1->GetCount());
    analysisManager->FillH1(1, energy, det2->GetCount());

    analysisManager->FillH1(2, angle, det1->GetCount());
    analysisManager->FillH1(3, angle, det2->GetCount());
}