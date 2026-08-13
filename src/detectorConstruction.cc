#include "detectorConstruction.hh"
#include "detectorGeometryDefault.hh"
#include "detectorGeometryDeVito.hh"
#include "DeVITOMagneticField.hh"

#include "G4RotationMatrix.hh"
#include "G4Exception.hh"

#include "VITOMagneticField.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"

MyDetectorConstruction::MyDetectorConstruction(std::string sampleType, double sampleThickness, double liquidThickness, double sampleDiameter, std::string detector, std::string magField) 
: fSampleType(sampleType), fSampleThickness(sampleThickness), fLiquidThickness(liquidThickness), fSampleDiameter(sampleDiameter), fDetector(detector), fRot(nullptr), fMagField(magField)
{}

MyDetectorConstruction::~MyDetectorConstruction()
{
    delete fRot;
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //====================================================================================
    //                              Defining World
    //====================================================================================
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    G4Box *solidWorld = new G4Box("solidWorld", 0.7*m, 0.7*m, 0.7*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *World = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
    logicWorld, "World", 0, false, 0, true);



    if(fSampleType == "solid_MgO" && fSampleThickness != 0.0){
    //====================================================================================
    //                              Defining Solid Sample
    //====================================================================================
    //---------------MgO---------------------------
        G4Material *solidSampleMat = nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
        G4Tubs *solidSampleCylinder = new G4Tubs("solidSampleCylinder", 0., fSampleDiameter/2.0*mm, fSampleThickness/2.0*mm, 0.*deg, 360.*deg);
        G4LogicalVolume *logicSampleCylinder = new G4LogicalVolume(solidSampleCylinder, solidSampleMat, 
                                                                "logicSampleCylinder");
        G4VPhysicalVolume *solidCylinder = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicSampleCylinder, 
                                                                "solidCylinder", logicWorld, false, 0, true);                                                      
    }

    //---------------KCl----------------------
    else if (fSampleType == "solid_KCl" && fSampleThickness != 0.0) {
        G4cout << "----------------------KCl-----------------" << G4endl;
        
        G4Element* K = nist->FindOrBuildElement("K");
        G4Element* Cl = nist->FindOrBuildElement("Cl");
        G4double density = 1.984 * g/cm3;  
        G4Material* KCl = new G4Material("KCl", density, 2);
        KCl->AddElement(K, 1);
        KCl->AddElement(Cl, 1);

        G4Tubs *solidSampleCylinder = new G4Tubs("solidSampleCylinder", 0., fSampleDiameter/2.0*mm, fSampleThickness/2.0*mm, 0.*deg, 360.*deg);
        G4LogicalVolume *logicSampleCylinder = new G4LogicalVolume(solidSampleCylinder, KCl, 
                                                                "logicSampleCylinder");
        G4VPhysicalVolume *solidCylinder = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicSampleCylinder, 
                                                            "solidCylinder", logicWorld, false, 0, true);
        
    }
    
    
    //====================================================================================
    //                              Defining Liquid Sample
    //====================================================================================
    //-------------Make muscovite mica disc------------------
    else if(fSampleType == "liquid"){
        G4Element* K = nist->FindOrBuildElement("K");
        G4Element* Al = nist->FindOrBuildElement("Al");
        G4Element* Si = nist->FindOrBuildElement("Si");
        G4Element* O = nist->FindOrBuildElement("O");
        G4Element* H = nist->FindOrBuildElement("H");
        
        G4Material* mica = new G4Material("Mica", 2.83*g/cm3, 5);

        mica->AddElement(K, 1);
        mica->AddElement(Al, 3);
        mica->AddElement(Si, 3);
        mica->AddElement(O, 12);
        mica->AddElement(H, 2);

        G4Tubs *liquidSampleDisc = new G4Tubs("liquidSampleDisc", 0., fSampleDiameter/2.0*mm, fSampleThickness/2.0*mm,
                                            0.*deg, 360.*deg);
        G4LogicalVolume *logicSampleDisc = new G4LogicalVolume(liquidSampleDisc, mica,  
                                                                    "logicSampleDisc");

        fRot = new G4RotationMatrix();
        fRot->rotateX(-45.*deg);
        G4double d = fSampleThickness/2.0 + fLiquidThickness/2.0;
        G4double y = d * std::sin(45.*deg);
        G4double z = -d * std::cos(45.*deg);
        
        G4VPhysicalVolume *liquidDisc = new G4PVPlacement(fRot, G4ThreeVector(0., 0., 0.), logicSampleDisc, 
                                                                    "liquidDisc", logicWorld, false, 0, true);


        //-------------Make EMIM mica disc------------------
        G4Element* C = nist->FindOrBuildElement("C");
        G4Element* N = nist->FindOrBuildElement("N");
        
        
        G4Material* EMIMDCA = new G4Material("EMIMDCA", 1.06*g/cm3, 3);

        EMIMDCA->AddElement(C, 8);
        EMIMDCA->AddElement(H, 11);
        EMIMDCA->AddElement(N, 5);


        G4Tubs *liquidSampleEMIM = new G4Tubs("liquidSampleEMIM", 0., fSampleDiameter/2.0*mm, fLiquidThickness/2.0*mm,
                                            0.*deg, 360.*deg);
        G4LogicalVolume *logicSampleEMIM = new G4LogicalVolume(liquidSampleEMIM, EMIMDCA,  
                                                                    "logicSampleEMIM");
                                                            
        G4VPhysicalVolume *liquidEMIM = new G4PVPlacement(fRot, G4ThreeVector(0., y, z), logicSampleEMIM, 
                                                                    "liquidEMIM", logicWorld, false, 0, true);

    }

    else if (fSampleThickness == 0.0) {
        G4cout << "------------------There is no sample------------------" << G4endl;
    }

    else {
        G4Exception("MyDetectorConstruction::Construct()", "InvalidSampleType", FatalException, ("Unknown sampleType: " + fSampleType).c_str());
    }
    
    

    //====================================================================================
    //                              Defining Detector
    //====================================================================================
    std::unique_ptr<DetectorGeometryDefault> detectorGeom;

    if (fDetector == "default") {
        detectorGeom = std::make_unique<DetectorGeometryDefault>();
    }

    // else if (fDetector == "devito") {
    //     detectorGeom = std::make_unique<DetectorGeometryDeVITO>();
    // }

    else if (fDetector == "devito2023" || fDetector == "devito2024" || fDetector == "devitoCircle2023" || fDetector == "devitoCircle2024") {
        detectorGeom = std::make_unique<DetectorGeometryDeVito>(fDetector);
    }

    else {G4Exception("MyDetectorConstruction::Construct()",
                "Wrong Detector",
                FatalException,
                "Choose detector default or devito in run.sh"); 
        }


    detectorGeom->Build(logicWorld);
    

    
    //====================================================================================
    //                             Activating the Magnetic Field
    //====================================================================================
    
    if (fMagField == "vito") {
        G4MagneticField* magField = new VITOMagneticField("Field/field1Axial.txt",
                                                      "Field/field1Radial.txt", 
                                                      "Field/field2Axial.txt", 
                                                      "Field/field2Radial.txt");
        fField.Put(magField);

        G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        pFieldMgr->SetDetectorField(fField.Get());
        pFieldMgr->CreateChordFinder(fField.Get());
    }


    else if (fMagField == "devito") {
        G4MagneticField* magField = new DeVITOMagneticField("Field/Field-Map_DeVITO.txt");
        fField.Put(magField);

        G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        pFieldMgr->SetDetectorField(fField.Get());
        pFieldMgr->CreateChordFinder(fField.Get());
    }

    else {G4Exception("MyDetectorConstruction::Construct()",
                "Wrong MagField",
                FatalException,
                "Choose magnetic field vito or devito in run.sh"); 
    }
    




    return World;
}