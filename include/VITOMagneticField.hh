#ifndef VITOMAGNETICFIELD_HH
#define VITOMAGNETICFIELD_HH

#include "G4MagneticField.hh"
#include "G4String.hh"

#include <vector>

/**
 * @class VITOMagneticField
 * @brief Custom Geant4 magnetic field that recreates a 2D cylindrical magnetic field
 *        from measured/calculated data files.
 *
 * This class reads axial (Bz) and radial (Br) magnetic field component grids
 * from 4 input files divided into 2 spatial regions:
 * - Region 1: Axial coordinate Z in [0, 0.45] meters, Radial R in [0, 0.078] meters.
 * - Region 2: Axial coordinate Z in [0.45, 1.00] meters, Radial R in [0, 0.55] meters.
 *
 * The class uses 2D bilinear interpolation to compute the field values at any point.
 * By default, it assumes symmetry about the Z = 0 plane (i.e. Bz is symmetric, Br is antisymmetric).
 */
class VITOMagneticField : public G4MagneticField {
public:
    /**
     * @brief Constructor
     * @param f1Axial Path to Region 1 Axial (Bz) field file
     * @param f1Radial Path to Region 1 Radial (Br) field file
     * @param f2Axial Path to Region 2 Axial (Bz) field file
     * @param f2Radial Path to Region 2 Radial (Br) field file
     * @param symmetric If true, assumes symmetry about Z = 0 plane.
     */
    VITOMagneticField(const G4String& f1Axial, const G4String& f1Radial,
                      const G4String& f2Axial, const G4String& f2Radial,
                      G4bool symmetric = true);
    
    virtual ~VITOMagneticField() override;

    /**
     * @brief Computes the magnetic field value at a given space-time point.
     * @param Point Space-time coordinates (x, y, z, t) in Geant4 internal units (mm, ns)
     * @param Bfield Out array of magnetic field components (Bx, By, Bz) in Geant4 units (Tesla)
     */
    virtual void GetFieldValue(const G4double Point[4], G4double* Bfield) const override;

private:
    struct GridData {
        std::vector<G4double> z_coords;
        std::vector<G4double> r_coords;
        std::vector<std::vector<G4double>> field_values;
        G4double z_min = 0.0;
        G4double z_max = 0.0;
        G4double r_min = 0.0;
        G4double r_max = 0.0;
        G4double z_step = 0.0;
        G4double r_step = 0.0;
        G4bool is_uniform = false;
    };

    /**
     * @brief Loads grid coordinates and field values from a text file.
     */
    G4bool LoadFieldFile(const G4String& filename, GridData& grid);

    /**
     * @brief Performs 2D bilinear interpolation on the grid data.
     */
    G4bool Interpolate2D(const GridData& axial, const GridData& radial,
                          G4double z, G4double r,
                          G4double& Bz, G4double& Br) const;

    GridData fRegion1Axial;
    GridData fRegion1Radial;
    GridData fRegion2Axial;
    GridData fRegion2Radial;

    G4bool fSymmetric;
    G4bool fLoadedSuccessfully;
};

#endif // VITO_MAGNETIC_FIELD_HH
