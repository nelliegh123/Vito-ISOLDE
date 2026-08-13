#include "detectorGeometryDeVito.hh"
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

DetectorGeometryDeVito::DetectorGeometryDeVito(std::string detector) : fDetector(detector)
{}
 
DetectorGeometryDeVito::~DetectorGeometryDeVito() = default;
 
 
void DetectorGeometryDeVito::Build(G4LogicalVolume* logicWorld)
{
    G4NistManager *nist = G4NistManager::Instance();
 
    G4double density;
    G4int numberElements;
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
 
    // EJ200 (used for 2023 front + back detectors, and 2024 back detector)
    G4Material* ej200 = new G4Material("ej200", density = 1.023 * g / cm3, numberElements = 2);
    ej200->AddElement(C, 469);
    ej200->AddElement(H, 517);
 
    // EJ204 (used for 2024 front detector)
    G4Material* ej204 = new G4Material("ej204", density = 1.023 * g / cm3, numberElements = 2);
    ej204->AddElement(C, 468);
    ej204->AddElement(H, 515);
  
 
    G4double corner_dz = 8 * mm, cornerside = 17.7 * mm;   
    G4double angle = twopi / 8;

    auto corner = new G4Box("corner1", 0.5 * cornerside, 0.5 * cornerside, 0.5 * corner_dz);
    
    G4RotationMatrix* rotm1 = new G4RotationMatrix();
    rotm1->rotateZ(45. * deg);
    G4RotationMatrix* rotm2 = new G4RotationMatrix();
    rotm2->rotateZ(135. * deg);
    G4RotationMatrix* rotm3 = new G4RotationMatrix();
    rotm3->rotateZ(225. * deg);
    G4RotationMatrix* rotm4 = new G4RotationMatrix();
    rotm4->rotateZ(315. * deg);
    
    G4VisAttributes* BetaDetVis = new G4VisAttributes(G4Colour(0.8, 0.2, 0.0));
    BetaDetVis->SetForceAuxEdgeVisible(true);
    BetaDetVis->SetForceSolid(true);
    
    G4LogicalVolume* logicDetector1 = nullptr;
    G4LogicalVolume* logicDetector2 = nullptr;
    G4double zOffset = 0;


    if (fDetector == "devito2023") {
        G4double betaside = 62 * mm, beta_dz = 6.3 * mm;   // beta detector dimensions
        G4double diameter = 10 * mm;                        // hole dimension
    
        // ---- FRONT DETECTOR ----
        auto solidBetaF = new G4Box("betaF", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dz);
        auto holeF = new G4Tubs("holeF", 0, 0.5 * diameter, corner_dz, 0, twopi);
    
        G4SubtractionSolid* volume1 = new G4SubtractionSolid("volume1", solidBetaF, holeF);
        G4SubtractionSolid* volume2 = new G4SubtractionSolid("volume2", volume1, corner, rotm1, G4ThreeVector(31, 31, 0));
        G4SubtractionSolid* volume3 = new G4SubtractionSolid("volume3", volume2, corner, rotm2, G4ThreeVector(-31, 31, 0));
        G4SubtractionSolid* volume4 = new G4SubtractionSolid("volume4", volume3, corner, rotm3, G4ThreeVector(-31, -31, 0));
        G4SubtractionSolid* volume5 = new G4SubtractionSolid("volume5", volume4, corner, rotm4, G4ThreeVector(31, -31, 0));
    
        // ---- BACK DETECTOR ----
        auto solidBetaB = new G4Box("betaB", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dz);
        auto holeB = new G4Tubs("holeB", 0, 0.5 * diameter, corner_dz, 0, twopi);
    
        G4SubtractionSolid* volume6 = new G4SubtractionSolid("volume6", solidBetaB, holeB);
        G4SubtractionSolid* volume7 = new G4SubtractionSolid("volume7", volume6, corner, rotm1, G4ThreeVector(31, 31, 0));
        G4SubtractionSolid* volume8 = new G4SubtractionSolid("volume8", volume7, corner, rotm2, G4ThreeVector(-31, 31, 0));
        G4SubtractionSolid* volume9 = new G4SubtractionSolid("volume9", volume8, corner, rotm3, G4ThreeVector(-31, -31, 0));
        G4SubtractionSolid* volume10 = new G4SubtractionSolid("volume10", volume9, corner, rotm4, G4ThreeVector(31, -31, 0));
    
        logicDetector1 = new G4LogicalVolume(volume5, ej200, "logicDetector1");
        logicDetector2 = new G4LogicalVolume(volume10, ej200, "logicDetector2");
    
        zOffset = 29.65 * mm;
    }



    else if (fDetector == "devito2024") {
        G4double betaside = 62 * mm, beta_dz = 6.3 * mm;   // back detector keeps 2023 dimensions
        G4double diameter = 10 * mm;                        // back detector hole
    
        G4double beta_dzF = 5 * mm;      // front detector thickness (thinner)
        G4double diameterF = 20 * mm;    // front detector hole (bigger aperture)
    
        // ---- FRONT DETECTOR ----
        auto solidBetaF = new G4Box("betaF", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dzF);
        auto holeF = new G4Tubs("holeF", 0, 0.5 * diameterF, corner_dz, 0, twopi);
    
        G4SubtractionSolid* volume1 = new G4SubtractionSolid("volume1", solidBetaF, holeF);
        G4SubtractionSolid* volume2 = new G4SubtractionSolid("volume2", volume1, corner, rotm1, G4ThreeVector(31, 31, 0));
        G4SubtractionSolid* volume3 = new G4SubtractionSolid("volume3", volume2, corner, rotm2, G4ThreeVector(-31, 31, 0));
        G4SubtractionSolid* volume4 = new G4SubtractionSolid("volume4", volume3, corner, rotm3, G4ThreeVector(-31, -31, 0));
        G4SubtractionSolid* volume5 = new G4SubtractionSolid("volume5", volume4, corner, rotm4, G4ThreeVector(31, -31, 0));
    
        // ---- BACK DETECTOR ----
        auto solidBetaB = new G4Box("betaB", 0.5 * betaside, 0.5 * betaside, 0.5 * beta_dz);
        auto holeB = new G4Tubs("holeB", 0, 0.5 * diameter, corner_dz, 0, twopi);
    
        G4SubtractionSolid* volume6 = new G4SubtractionSolid("volume6", solidBetaB, holeB);
        G4SubtractionSolid* volume7 = new G4SubtractionSolid("volume7", volume6, corner, rotm1, G4ThreeVector(31, 31, 0));
        G4SubtractionSolid* volume8 = new G4SubtractionSolid("volume8", volume7, corner, rotm2, G4ThreeVector(-31, 31, 0));
        G4SubtractionSolid* volume9 = new G4SubtractionSolid("volume9", volume8, corner, rotm3, G4ThreeVector(-31, -31, 0));
        G4SubtractionSolid* volume10 = new G4SubtractionSolid("volume10", volume9, corner, rotm4, G4ThreeVector(31, -31, 0));
    
        logicDetector1 = new G4LogicalVolume(volume5, ej204, "logicDetector1");
        logicDetector2 = new G4LogicalVolume(volume10, ej200, "logicDetector2");
    
        zOffset = 29 * mm;
    }


    if (!logicDetector1 || !logicDetector2) {
        G4Exception("DetectorGeometryDeVITOCircle::Build", "InvalidDetector",
                    FatalException, ("Unknown fDetector value: " + fDetector).c_str());
    }
    
    logicDetector1->SetVisAttributes(BetaDetVis);
    logicDetector2->SetVisAttributes(BetaDetVis);
    
    // ---------------------------------------------------------------------
    // PLACEMENT (common for both years, uses year-specific zOffset)
    // ---------------------------------------------------------------------
    G4VPhysicalVolume* Detector = new G4PVPlacement(nullptr,
                                                    G4ThreeVector(0, 0, +zOffset),
                                                    logicDetector1,
                                                    "Detector1",
                                                    logicWorld,
                                                    false,
                                                    0,
                                                    true);
    
    G4VPhysicalVolume* Detector2 = new G4PVPlacement(nullptr,
                                                    G4ThreeVector(0, 0, -zOffset),
                                                    logicDetector2,
                                                    "Detector2",
                                                    logicWorld,
                                                    false,
                                                    1,
                                                    true);
    
    // ---------------------------------------------------------------------
    // SENSITIVE DETECTORS (common for both years)
    // ---------------------------------------------------------------------
    auto sdManager = G4SDManager::GetSDMpointer();
    auto sd1 = new MySensitiveDetector("Detector1SD");
    auto sd2 = new MySensitiveDetector("Detector2SD");
    sdManager->AddNewDetector(sd1);
    sdManager->AddNewDetector(sd2);
    logicDetector1->SetSensitiveDetector(sd1);
    logicDetector2->SetSensitiveDetector(sd2);
 
    
}
 