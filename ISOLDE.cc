#include <iostream>
#include "G4RunManager.hh"
#include "G4UImanager.hh" 
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physicsList.hh"
#include "actionInitialization.hh"
#include "eventAction.hh"

#define USE_GPS


int main(int argc, char** argv)
{
    bool useGUI = false;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--gui") {
            useGUI = true;
        }
    }

    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    runManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    G4UIExecutive* ui = nullptr;
    G4VisManager* visManager = nullptr;

    if (useGUI) {
        ui = new G4UIExecutive(argc, argv);

        visManager = new G4VisExecutive();
        visManager->Initialize();

        UImanager->ApplyCommand("/vis/open OGL");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    }

#ifdef USE_GPS
    // UImanager->ApplyCommand("/control/execute energy_scan.mac");
    UImanager->ApplyCommand("/control/execute angle_scan.mac");
#else
    runManager->BeamOn(100);
#endif

    if (useGUI && ui) {
        ui->SessionStart();
    }

    delete visManager;
    delete ui;
    delete runManager;

    return 0;
}