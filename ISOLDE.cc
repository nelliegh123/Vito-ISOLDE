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
    if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <macroFile> <sampleType> <sampleThickness> <liquidThickness> [--gui]" << std::endl;
    return 1;
    }

    bool useGUI = false;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--gui") {
            useGUI = true;
        }
    }

    std::string sampleType = argv[2];
    double sampleThickness = std::stod(argv[3]);
    double liquidThickness = std::stod(argv[4]);

    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction(sampleType, sampleThickness, liquidThickness));
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
    // UImanager->ApplyCommand("/control/macroPath macros");
    // UImanager->ApplyCommand("/control/execute energy_scan.mac");


    
    if (argc > 1) {
        G4String macroFile = argv[1];

        UImanager->ApplyCommand("/control/macroPath runScripts");
        UImanager->ApplyCommand("/control/execute " + macroFile);
    }
    else {
        G4cerr << "No macro file provided!" << G4endl;
    }



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