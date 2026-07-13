#include "construction.hh"
#include "sensitiveDetector.hh"

#include "VITOMagneticField.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"

MyDetectorConstruction::MyDetectorConstruction(std::string sampleType, double sampleThickness) 
: fSampleType(sampleType), fSampleThickness(sampleThickness)
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

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



    if(fSampleType == "solid"){
    //====================================================================================
    //                              Defining Solid Sample
    //====================================================================================
        G4Material *solidSampleMat = nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");

        G4Tubs *solidSampleCylinder = new G4Tubs("solidSampleCylinder", 0., 0.5*cm, fSampleThickness/2*mm, 0.*deg, 360.*deg);
        G4Box *solidSampleSquare = new G4Box("solidSampleSquare", 0.5*cm, 0.5*cm, fSampleThickness/2*mm);

        G4LogicalVolume *logicSampleCylinder = new G4LogicalVolume(solidSampleCylinder, solidSampleMat, 
                                                                "logicSampleCylinder");
        G4LogicalVolume *logicSampleSquare = new G4LogicalVolume(solidSampleSquare, solidSampleMat, 
                                                                "logicSampleSquare");

        G4VPhysicalVolume *solidCylinder = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicSampleCylinder, 
                                                                "solidCylinder", logicWorld, false, 0, true);
        // G4VPhysicalVolume *solidSquare = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicSampleSquare, 
                                                                //   "solidSquare", logicWorld, false, 0, true);
    }
    
    else if(fSampleType == "liquid"){
    //====================================================================================
    //                              Defining Liquid Sample
    //====================================================================================
    //-------------Make muscovite mica disc------------------
    G4Element* K = nist->FindOrBuildElement("K");
    G4Element* Al = nist->FindOrBuildElement("Al");
    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* H = nist->FindOrBuildElement("H");
    
    G4Material* mica = new G4Material("Mica", 2.83*g/cm, 5);

    mica->AddElement(K, 1);
    mica->AddElement(Al, 3);
    mica->AddElement(Si, 3);
    mica->AddElement(O, 12);
    mica->AddElement(H, 2);



    }

    else {
        G4Exception("MyDetectorConstruction::Construct()", "InvalidSampleType", FatalException, ("Unknown sampleType: " + fSampleType).c_str());
    }
    
    

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

    logicDetector1->SetSensitiveDetector(sd1);
    logicDetector2->SetSensitiveDetector(sd2);

    

    
    //====================================================================================
    //                             Activating the Magnetic Field
    //====================================================================================
    G4MagneticField* magField = new VITOMagneticField("Field/field1Axial.txt",
                                                      "Field/field1Radial.txt", 
                                                      "Field/field2Axial.txt", 
                                                      "Field/field2Radial.txt");
    fField.Put(magField);

    G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    pFieldMgr->SetDetectorField(fField.Get());
    pFieldMgr->CreateChordFinder(fField.Get());




    return World;
}