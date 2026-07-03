#include "runAction.hh"
#include "G4AnalysisManager.hh"
#include "primaryGeneratorAction.hh"
#include "G4Run.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    // analysisManager->CreateH1("Detector1_Scan", "", 19, 0.75, 10.25);
    // analysisManager->CreateH1("Detector2_Scan", "", 19, 0.75, 10.25);
    analysisManager->CreateH1("Detector1_Scan", "", 10, 0.5, 10.5);
    analysisManager->CreateH1("Detector2_Scan", "", 10, 0.5, 10.5);
    // analysisManager->CreateH1("Detector1_Scan_Angle", "Detector 1 angle", 38, 1.956521739, 182);
    // analysisManager->CreateH1("Detector2_Scan_Angle", "Detector 2 angle", 38, 2, 181.956521739);
    analysisManager->CreateH1("Detector1_Scan_Angle", "Detector 2 angle", 50, -2, 182);
    analysisManager->CreateH1("Detector2_Scan_Angle", "Detector 2 angle", 50, -2, 182);
    
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