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
    analysisManager->CreateNtupleDColumn("rmEnergy"); 
    analysisManager->FinishNtuple();    

    // analysisManager->CreateNtuple("removed", "Removed data");
    // analysisManager->CreateNtupleDColumn("rmPosX");
    // analysisManager->CreateNtupleDColumn("rmPosY");
    // analysisManager->CreateNtupleDColumn("rmPosZ");
    // analysisManager->CreateNtupleDColumn("rmAngle");
    // analysisManager->CreateNtupleDColumn("rmEnergy");
    // analysisManager->FinishNtuple();   // ntuple id 1
}       


void runAction::BeginOfRunAction(const G4Run*) 
{}

void runAction::EndOfRunAction(const G4Run* run)
{}