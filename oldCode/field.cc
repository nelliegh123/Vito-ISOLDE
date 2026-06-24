#include "field.hh"

#include <fstream>
#include <sstream>

VITOMagneticField::VITOMagneticField(const G4String& f1A,
                                     const G4String& f1R,
                                     const G4String& f2A,
                                     const G4String& f2R)
{
    LoadFieldFile(F1A, field1Axial);
    LoadFieldFile(F1R, field1Radial);
    LoadFieldFile(F2A, field2Axial);
    LoadFieldFile(F2R, field2Radial);
}

void VITOMagneticField::LoadFieldFile(const G4String& filename, std::vector<double>& storage)
{
    std::ifstream file(filename);
    double value;
    while (file >> value)
        storage.push_back(value);
}

void VITOMagneticField::GetFieldValue(const G4double point[4], G4double* B) const 
{
    B[0] = 0.0;
    B[1] = 0.0;
    B[2] = field1Axial[0];  // placeholder
}

