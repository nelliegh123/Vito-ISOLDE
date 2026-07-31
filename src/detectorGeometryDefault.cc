#include "detectorGeometryDefault.hh"
#include "sensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

void DetectorGeometryDefault::Build(G4LogicalVolume* logicWorld)
{

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *detectorMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    //Defining the front detector
    G4Tubs *solidDetector1 = new G4Tubs("solidDetector1", 7.*mm, 8.5*cm, 1.*cm,
                                          0.*cm, 360.*cm);
    //Defining the back detector                                  
    G4Tubs *solidDetector2 = new G4Tubs("solidDetector2", 0.*cm, 2*cm, 1.*cm,
                                          0.*cm, 360.*cm);

    G4LogicalVolume *logicDetector1 = new G4LogicalVolume(solidDetector1, detectorMat, "logicDetector1");
    G4LogicalVolume *logicDetector2 = new G4LogicalVolume(solidDetector2, detectorMat, "logicDetector2");


    G4VPhysicalVolume *Detector = new G4PVPlacement(0, G4ThreeVector(0.0*meter, 0.0*meter, -600.*mm), 
                                    logicDetector1, "Detector1", logicWorld, false, 0, true);

    G4VPhysicalVolume *Detector2 = new G4PVPlacement(0, G4ThreeVector(0.0*meter, 0.0*meter, 270.*mm), 
                                    logicDetector2, "Detector2", logicWorld, true, 1, true);

    auto sdManager = G4SDManager::GetSDMpointer();
    auto sd1 = new MySensitiveDetector("Detector1SD");
    auto sd2 = new MySensitiveDetector("Detector2SD");
    sdManager->AddNewDetector(sd1);
    sdManager->AddNewDetector(sd2);

    logicDetector1->SetSensitiveDetector(sd1);
    logicDetector2->SetSensitiveDetector(sd2);
}
