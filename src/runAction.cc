#include "runAction.hh"
#include "G4AnalysisManager.hh"
#include "primaryGeneratorAction.hh"
#include "G4Run.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(2);
    

    // ----------Energy Scan--------------
    analysisManager->CreateH1("Detector1_Scan", "", 19, 0.75, 10.25);
    analysisManager->CreateH1("Detector2_Scan", "", 19, 0.75, 10.25);
    // analysisManager->CreateH1("Detector1_Scan", "", 10, 0.5, 10.5);
    // analysisManager->CreateH1("Detector2_Scan", "", 10, 0.5, 10.5);

    // ----------Angle Scan--------------
    analysisManager->CreateH1("Detector1_Scan_Angle", "Detector 2 angle", 46, -2, 182);
    analysisManager->CreateH1("Detector2_Scan_Angle", "Detector 2 angle", 46, -2, 182);


    // ----------Energy and Angle Scan--------------
    G4int id1 = analysisManager->CreateH2("Detector_E_vs_Angle_1", "Detector 1",
    46, -2, 182,       // x-axis (angle)
    19, 0.75, 10.25);    // y-axis (energy)
    G4int id2 = analysisManager->CreateH2("Detector_E_vs_Angle_2", "Detector 2",
    46, -2, 182,       // x-axis (angle)
    19, 0.75, 10.25);    // y-axis (energy)

    G4cout << "--------------------------------------H2 IDs------------------: " << id1 << " " << id2 << G4endl;
    
    
}

void runAction::BeginOfRunAction(const G4Run*) { //Now running this from macro
    // auto analysisManager = G4AnalysisManager::Instance();
    // analysisManager->OpenFile("SimulationResults.root");
}

void runAction::EndOfRunAction(const G4Run* run) { //Now running this from macro
    // auto analysisManager = G4AnalysisManager::Instance();
    // analysisManager->Write();
    // analysisManager->CloseFile();


    // fNEvents = run->GetNumberOfEvent();
    // G4cout <<fNEvents << G4endl;

}