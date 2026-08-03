#include "detectorGeometryDeVITO.hh"
#include "sensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

 
#include "G4PhysicalConstants.hh"  // twopi
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// void DetectorGeometryDeVITO::Build(G4LogicalVolume* logicWorld)
// {
//     G4cout << "---------------------------------------YOU ARE USING DeVITO-----------------------------------" << G4endl;
//     G4NistManager *nist = G4NistManager::Instance();
//     G4Material *detectorMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
//     //Defining the front detector
//     G4Tubs *solidDetector1 = new G4Tubs("solidDetector1", 7.*mm, 20*cm, 1.*cm,
//                                           0.*cm, 360.*cm);
//     //Defining the back detector                                  
//     G4Tubs *solidDetector2 = new G4Tubs("solidDetector2", 0.*cm, 20*cm, 1.*cm,
//                                           0.*cm, 360.*cm);
//     G4LogicalVolume *logicDetector1 = new G4LogicalVolume(solidDetector1, detectorMat, "logicDetector1");
//     G4LogicalVolume *logicDetector2 = new G4LogicalVolume(solidDetector2, detectorMat, "logicDetector2");
//     G4VPhysicalVolume *Detector = new G4PVPlacement(0, G4ThreeVector(0.0*meter, 0.0*meter, -600.*mm), 
//                                     logicDetector1, "Detector1", logicWorld, false, 0, true);
//     G4VPhysicalVolume *Detector2 = new G4PVPlacement(0, G4ThreeVector(0.0*meter, 0.0*meter, 270.*mm), 
//                                     logicDetector2, "Detector2", logicWorld, true, 1, true);
//     auto sdManager = G4SDManager::GetSDMpointer();
//     auto sd1 = new MySensitiveDetector("Detector1SD");
//     auto sd2 = new MySensitiveDetector("Detector2SD");
//     sdManager->AddNewDetector(sd1);
//     sdManager->AddNewDetector(sd2);
//     logicDetector1->SetSensitiveDetector(sd1);
//     logicDetector2->SetSensitiveDetector(sd2);
// }



void DetectorGeometryDeVITO::Build(G4LogicalVolume* logicWorld)
{
    G4cout << "---------------------------------------YOU ARE USING DeVITO-----------------------------------" << G4endl;

    // 2023 geometry; 2 identical detectors, made at CERN
    // COMMON FOR BOTH DETECTORS
    G4double betaside = 62 * mm, beta_dz = 6.3 * mm;              // beta detectors dimensions
    G4double corner_dz = 8 * mm, cornerside = 17.7 * mm;          // dimensions for corners which will be subtracted
    G4double angle = twopi / 8;
    G4double diameter = 10 * mm;                                  // dimensions for the hole

    // 2024 geometry; front is different (made at UTK) -> bigger aperture, smaller thickness, ej204
    // UNCOMMENT for FRONT DETECTOR 2024
    // G4double beta_dzF = 5 * mm;      // beta detector dimension
    // G4double diameterF = 20 * mm;    // dimension for the hole

    G4double atomicMass, z, density;
    G4int numberElements;

    // DEFINITION OF EJ200
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");

    G4Material* ej200 = new G4Material("ej200", density = 1.023 * g / cm3, numberElements = 2);
    ej200->AddElement(C, 469);
    ej200->AddElement(H, 517);

    // DEFINITION OF EJ204 (rename to avoid duplicate material name clash with ej200)
    G4Material* ej204 = new G4Material("ej204", density = 1.023 * g / cm3, numberElements = 2);
    ej204->AddElement(C, 468);
    ej204->AddElement(H, 515);

    G4cout << "DeVITO: materials done" << G4endl;




    // ---- FRONT DETECTOR ----
    // 2023 geometry - UNCOMMENT THE FOLLOWING 2 LINES FOR BETA FRONT 2023
    auto solidBetaF = new G4Box("betaF", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dz);
    auto holeF = new G4Tubs("holeF", 0, 0.5 * diameter, corner_dz, 0, twopi);
    // 2024 geometry - UNCOMMENT THE FOLLOWING 2 LINES FOR BETA FRONT 2024
    // auto solidBetaF = new G4Box("betaF", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dzF);
    // auto holeF = new G4Tubs("holeF", 0, 0.5 * diameterF, corner_dz, 0, twopi);

    auto corner = new G4Box("corner1", 0.5 * cornerside, 0.5 * cornerside, 0.5 * corner_dz);
    G4SubtractionSolid* volume1 = new G4SubtractionSolid("volume1", solidBetaF, holeF);

    G4RotationMatrix* rotm1 = new G4RotationMatrix();
    rotm1->rotateZ(45. * deg);
    G4RotationMatrix* rotm2 = new G4RotationMatrix();
    rotm2->rotateZ(135. * deg);
    G4RotationMatrix* rotm3 = new G4RotationMatrix();
    rotm3->rotateZ(225. * deg);
    G4RotationMatrix* rotm4 = new G4RotationMatrix();
    rotm4->rotateZ(315. * deg);

    G4SubtractionSolid* volume2 = new G4SubtractionSolid("volume2", volume1, corner, rotm1, G4ThreeVector(31, 31, 0));
    G4SubtractionSolid* volume3 = new G4SubtractionSolid("volume3", volume2, corner, rotm2, G4ThreeVector(-31, 31, 0));
    G4SubtractionSolid* volume4 = new G4SubtractionSolid("volume4", volume3, corner, rotm3, G4ThreeVector(-31, -31, 0));
    G4SubtractionSolid* volume5 = new G4SubtractionSolid("volume5", volume4, corner, rotm4, G4ThreeVector(31, -31, 0));


    G4cout << "DeVITO: solids/subtraction done" << G4endl;






    // ---- BACK DETECTOR ----
    auto solidBetaB = new G4Box("betaB", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dz);
    auto holeB = new G4Tubs("holeB", 0, 0.5 * diameter, corner_dz, 0, twopi);
    G4SubtractionSolid* volume6 = new G4SubtractionSolid("volume6", solidBetaB, holeB);
    G4SubtractionSolid* volume7 = new G4SubtractionSolid("volume7", volume6, corner, rotm1, G4ThreeVector(31, 31, 0));
    G4SubtractionSolid* volume8 = new G4SubtractionSolid("volume8", volume7, corner, rotm2, G4ThreeVector(-31, 31, 0));
    G4SubtractionSolid* volume9 = new G4SubtractionSolid("volume9", volume8, corner, rotm3, G4ThreeVector(-31, -31, 0));
    G4SubtractionSolid* volume10 = new G4SubtractionSolid("volume10", volume9, corner, rotm4, G4ThreeVector(31, -31, 0));

    G4VisAttributes* BetaDetVis = new G4VisAttributes(G4Colour(0.8, 0.2, 0.0));
    BetaDetVis->SetForceAuxEdgeVisible(true);
    BetaDetVis->SetForceSolid(true);

    // logicDetector1 = front, logicDetector2 = back (keeping DeVITO naming convention)
    G4LogicalVolume* logicDetector1 = new G4LogicalVolume(volume5,   // its solid
                                                            ej200,    // CHANGE ej200 FOR 2023, ej204 FOR 2024
                                                            "logicDetector1");
    logicDetector1->SetVisAttributes(BetaDetVis);

    G4LogicalVolume* logicDetector2 = new G4LogicalVolume(volume10,  // its solid
                                                            ej200,    // ej200 FOR 2023
                                                            "logicDetector2");
    logicDetector2->SetVisAttributes(BetaDetVis);

    // ---- PLACEMENT ----
    G4VPhysicalVolume* Detector = new G4PVPlacement(nullptr,                        // no rotation
                                                      G4ThreeVector(0, 0, -29.65 * mm),  // CHANGE TO 29.65 if 2023, 29 if 2024
                                                      logicDetector1,
                                                      "Detector1",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      true);

    G4VPhysicalVolume* Detector2 = new G4PVPlacement(nullptr,                        // no rotation
                                                       G4ThreeVector(0, 0, +29.65 * mm),
                                                       logicDetector2,
                                                       "Detector2",
                                                       logicWorld,
                                                       false,
                                                       1,
                                                       true);

    // ---- SENSITIVE DETECTORS ----
    auto sdManager = G4SDManager::GetSDMpointer();
    auto sd1 = new MySensitiveDetector("Detector1SD");
    auto sd2 = new MySensitiveDetector("Detector2SD");
    sdManager->AddNewDetector(sd1);
    sdManager->AddNewDetector(sd2);
    logicDetector1->SetSensitiveDetector(sd1);
    logicDetector2->SetSensitiveDetector(sd2);
}
