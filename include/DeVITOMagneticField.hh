#ifndef DEVITOMAGNETICFIELD_HH
#define DEVITOMAGNETICFIELD_HH

#include "G4MagneticField.hh"
#include "G4String.hh"
#include <vector>

/**
 * @class DeVITOMagneticField
 * @brief Custom Geant4 magnetic field that reads a 3D magnetic field map
 *        from a text file (x y z Bx By Bz in mm and T) and performs trilinear interpolation.
 */
class DeVITOMagneticField : public G4MagneticField {
public:
    DeVITOMagneticField(const G4String& filename);
    virtual ~DeVITOMagneticField() override;

    virtual void GetFieldValue(const G4double Point[4], G4double* Bfield) const override;

private:
    G4bool LoadFieldFile(const G4String& filename);

    // Grid coordinates
    std::vector<G4double> fXCoords;
    std::vector<G4double> fYCoords;
    std::vector<G4double> fZCoords;

    // Field components stored in flat 1D vectors
    std::vector<G4double> fBx;
    std::vector<G4double> fBy;
    std::vector<G4double> fBz;

    // Grid limits
    G4double fXMin, fXMax;
    G4double fYMin, fYMax;
    G4double fZMin, fZMax;

    // Step sizes
    G4double fDx, fDy, fDz;

    // Grid dimensions
    size_t fNx, fNy, fNz;

    G4bool fIsUniform;
    G4bool fLoadedSuccessfully;
};

#endif // DEVITOMAGNETICFIELD_HH