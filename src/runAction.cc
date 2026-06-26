#include "runAction.hh"
#include "G4AnalysisManager.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    analysisManager->CreateH1("Detector1_Scan", "Detector 1", 19, 0.75, 10.25);
    analysisManager->CreateH1("Detector2_Scan", "Detector 2", 19, 0.75, 10.25);
    analysisManager->CreateH1("Detector1_Scan_Angle", "Detector 1 angle", 19, 0.5, 20.5);
    analysisManager->CreateH1("Detector2_Scan_Angle", "Detector 2 angle", 19, 0.5, 20.5);
}

void runAction::BeginOfRunAction(const G4Run*) { //Now running this from macro
    // auto analysisManager = G4AnalysisManager::Instance();
    // analysisManager->OpenFile("SimulationResults.root");
}

void runAction::EndOfRunAction(const G4Run*) { //Now running this from macro
    // auto analysisManager = G4AnalysisManager::Instance();

    // analysisManager->Write();
    // analysisManager->CloseFile();
}