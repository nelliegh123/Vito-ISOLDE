#include "steppingAction.hh"
#include "eventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include <chrono>

steppingAction::steppingAction(eventAction* evtAction) : fEventAction(evtAction) {}

steppingAction::~steppingAction() {}

void steppingAction::UserSteppingAction(const G4Step* step)
{
    auto now = std::chrono::steady_clock::now();
    double elapsedSec = std::chrono::duration<double>(
        now - fEventAction->GetStartTime()).count();

    
    if (elapsedSec > 1.0) {
        G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();

        G4double energy = 0.0;
        G4double angle = 0.0;
        const G4Event* event = G4EventManager::GetEventManager()->GetConstCurrentEvent();
        if (event && event->GetNumberOfPrimaryVertex() > 0) {
            auto primaryParticle = event->GetPrimaryVertex(0)->GetPrimary(0);
            if (primaryParticle) {
                energy = primaryParticle->GetKineticEnergy();
                G4ThreeVector dir = primaryParticle->GetMomentumDirection();
                angle = dir.theta() * 180.0 / 3.14159265358979323846;
            }
        }


        G4cout << "Event running too long (" << elapsedSec << " s) -- aborting.\n"
               << "  Position: (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ") mm\n"
               << "  Emission angle: " << angle << " deg\n"
               << "  Emission energy: " << energy << " MeV"
               << G4endl;

        step->GetTrack()->SetTrackStatus(fStopAndKill);
        G4RunManager::GetRunManager()->AbortEvent();
        return;
    }
}