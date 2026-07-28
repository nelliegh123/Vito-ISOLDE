#include "runAction.hh"
#include "G4AnalysisManager.hh"
#include "primaryGeneratorAction.hh"
#include "G4Run.hh"

runAction::runAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(2);

    analysisManager->CreateNtuple("hits", "Detector hits");
    analysisManager->CreateNtupleDColumn("energy");        
    analysisManager->CreateNtupleDColumn("angle");    
    analysisManager->CreateNtupleIColumn("detector"); 
    analysisManager->FinishNtuple();    
}

void runAction::BeginOfRunAction(const G4Run*) 
{}

void runAction::EndOfRunAction(const G4Run* run)
{}