#include "construction.hh"
#include "sensitiveDetector.hh"

#include "VITOMagneticField.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //====================================================================================
    //                              Defining World
    //====================================================================================
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box *solidWorld = new G4Box("solidWorld", 0.7*m, 0.7*m, 0.7*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *World = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
    logicWorld, "World", 0, false, 0, true);

    

    //====================================================================================
    //                              Defining Detector
    //====================================================================================
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

    //====================================================================================
    //                             Making Detectors Sensitive
    //====================================================================================
    auto sdManager = G4SDManager::GetSDMpointer();

    auto sd1 = new MySensitiveDetector("Detector1SD");
    auto sd2 = new MySensitiveDetector("Detector2SD");

    sdManager->AddNewDetector(sd1);
    sdManager->AddNewDetector(sd2);
//     G4Cache<G4MagneticField*> fFiwDetector(sd2);

    logicDetector1->SetSensitiveDetector(sd1);
    logicDetector2->SetSensitiveDetector(sd2);

    
    //====================================================================================
    //                             Activating the Magnetic Field
    //====================================================================================
    G4MagneticField* magField = new VITOMagneticField("./field1Axial.txt",
                                                      "./field1Radial.txt", 
                                                      "./field2Axial.txt", 
                                                      "./field2Radial.txt");
    fField.Put(magField);

    G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    pFieldMgr->SetDetectorField(fField.Get());
    pFieldMgr->CreateChordFinder(fField.Get());




    return World;
}