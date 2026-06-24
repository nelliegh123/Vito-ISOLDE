#ifndef FIELD_HH
#define FIELD_HH

#include "G4MagneticField.hh"
#include <vector>

class VITOMagneticField : public G4MagneticField
{
public:
    VITOMagneticField(const G4String& f1Axial,
                        const G4String& f1Radial,
                        const G4String& f2Axial,
                        const G4String& f2Radial);
    void GetFieldValue(const G4double point[4], G4double* Bfield) const override;

private:
    std::vector<double> field1Axial;
    std::vector<double> field1Radial;
    std::vector<double> field2Axial;
    std::vector<double> field2Radial;

    void LoadFielsFile(const G4String& filename, std::vector<double>& storage);
};

#endif 