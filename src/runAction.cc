#include "runAction.hh"
#include "G4AnalysisManager.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    
    analysisManager->CreateNtuple("HitsTree", "Simulation Results");
    analysisManager->CreateNtupleDColumn("Energy");   
    analysisManager->CreateNtupleIColumn("Det1Hits");  
    analysisManager->CreateNtupleIColumn("Det2Hits");  
    analysisManager->FinishNtuple();
}

void runAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile("SimulationResults.root");
}

void runAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}