#ifndef VITOMAGNETICFIELD_HH
#define VITOMAGNETICFIELD_HH

#include "G4MagneticField.hh"
#include "globals.hh"
#include <vector>
#include <string>

class VITOMagneticField : public G4MagneticField
{
public:
    VITOMagneticField(const G4String& file1,
                      const G4String& file2,
                      const G4String& file3,
                      const G4String& file4);

    virtual ~VITOMagneticField();

    virtual void GetFieldValue(const G4double point[4], G4double* Bfield) const override;

private:
    void ReadFieldMap(const G4String& file,
                      std::vector<G4double>& r,
                      std::vector<G4double>& z,
                      std::vector<std::vector<G4double>>& B);

    G4double Interpolate2D(G4double r, G4double z,
                           const std::vector<G4double>& R,
                           const std::vector<G4double>& Z,
                           const std::vector<std::vector<G4double>>& B) const;

    std::vector<G4double> fR1, fZ1;
    std::vector<std::vector<G4double>> fB1;

    std::vector<G4double> fR2, fZ2;
    std::vector<std::vector<G4double>> fB2;
};
#endif