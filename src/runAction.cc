#include "runAction.hh"
#include "G4AnalysisManager.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    analysisManager->CreateH1("Detector1_Scan", "Detector 1 Hits vs Energy;Energy (MeV);Total Hits", 10, 0.5, 10.5);
    analysisManager->CreateH1("Detector2_Scan", "Detector 2 Hits vs Energy;Energy (MeV);Total Hits", 10, 0.5, 10.5);
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