#include "construction.hh"

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

    G4Box *solidWorld = new G4Box("solidWorld", 1.0*m, 1.0*m, 1.0*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *World = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
    logicWorld, "World", 0, false, 0, true);

    

    //====================================================================================
    //                              Defining Detector
    //====================================================================================
    G4Material *detectorMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");


    G4double innerRadius = 0.*cm;
    G4double outerRadius = 60.*cm;
    G4double hz = 5.*cm;
    G4double startingAngle = 0.*cm;
    G4double spanningAngle = 360.*cm;
    G4Tubs *solidDetector = new G4Tubs("solidDetector", innerRadius, outerRadius, hz,
                                          startingAngle, spanningAngle);

    G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    G4double pos_x = 0.0*meter;
    G4double pos_y = 0.0*meter;
    G4double pos_z = 0.8*meter;
    G4VPhysicalVolume *Detector = new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                    logicDetector, "Detector", logicWorld, false, 0, true);

    G4VPhysicalVolume *Detector2 = new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, -pos_z), 
                                    logicDetector, "Detector2", logicWorld, true, 0, true);

    
    return World;
}