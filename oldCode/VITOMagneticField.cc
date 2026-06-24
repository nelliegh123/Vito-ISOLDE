#include "VITOMagneticField.hh"

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include <fstream>
#include <iostream>

VITOMagneticField::VITOMagneticField(const G4String& file1,
                                     const G4String& file2,
                                     const G4String& file3,
                                     const G4String& file4)
{
    ReadFieldMap(file1, fR1, fZ1, fB1);
    ReadFieldMap(file2, fR2, fZ2, fB2);
}

VITOMagneticField::~VITOMagneticField(){}


void VITOMagneticField::ReadFieldMap(const G4String& file,
                                     std::vector<G4double>& r,
                                     std::vector<G4double>& z,
                                     std::vector<std::vector<G4double>>& B)
{
    std::ifstream fin(file);

    std::string line;
    std::vector<std::vector<G4double>> temp;

    while (std::getline(fin, line))
    {
        std::istringstream ss(line);
        std::vector<G4double> row;
        G4double val;

        while (ss >> val)
        {
            row.push_back(val);
        }

        if (!row.empty())
            temp.push_back(row);
    }

    fin.close();

    r.assign(temp[0].begin() + 1, temp[0].end());

    for (size_t i = 1; i < temp.size(); i++)
    {
        z.push_back(temp[i][0]);

        std::vector<G4double> br(temp[i].begin() + 1, temp[i].end());
        B.push_back(br);
    }
}

G4double VITOMagneticField::Interpolate2D(
    G4double r, G4double z,
    const std::vector<G4double>& R,
    const std::vector<G4double>& Z,
    const std::vector<std::vector<G4double>>& B) const
{
    size_t iR = 0, iZ = 0;

    if (r <= R.front()) iR = 0;
    else if (r >= R.back()) iR = R.size() - 2;
    else
        while (iR + 1 < R.size() && R[iR + 1] < r) iR++;

    if (z <= Z.front()) iZ = 0;
    else if (z >= Z.back()) iZ = Z.size() - 2;
    else
        while (iZ + 1 < Z.size() && Z[iZ + 1] < z) iZ++;

    G4double r1 = R[iR], r2 = R[iR + 1];
    G4double z1 = Z[iZ], z2 = Z[iZ + 1];

    G4double Q11 = B[iZ][iR];
    G4double Q21 = B[iZ][iR + 1];
    G4double Q12 = B[iZ + 1][iR];
    G4double Q22 = B[iZ + 1][iR + 1];

    G4double fr = (r - r1) / (r2 - r1);
    G4double fz = (z - z1) / (z2 - z1);

    return (1 - fr) * (1 - fz) * Q11 +
           fr * (1 - fz) * Q21 +
           (1 - fr) * fz * Q12 +
           fr * fz * Q22;
}



void VITOMagneticField::GetFieldValue(const G4double point[4], G4double* Bfield) const
{
    G4double x = point[0];
    G4double y = point[1];
    G4double z = point[2];

    G4double r = std::sqrt(x*x + y*y);

    G4double Br = Interpolate2D(r, z, fR1, fZ1, fB1);
    G4double Bz = Interpolate2D(r, z, fR2, fZ2, fB2);

    if (r > 0)
    {
        G4double Bx = Br * x / r;
        G4double By = Br * y / r;

        Bfield[0] = Bx;
        Bfield[1] = By;
        Bfield[2] = Bz;
    }
    else
    {
        Bfield[0] = 0.0;
        Bfield[1] = 0.0;
        Bfield[2] = Bz;
    }
}