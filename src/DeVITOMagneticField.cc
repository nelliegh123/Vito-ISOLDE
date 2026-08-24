#include "DeVITOMagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cctype>

DeVITOMagneticField::DeVITOMagneticField(const G4String& filename)
    : fXMin(0.0), fXMax(0.0), fYMin(0.0), fYMax(0.0), fZMin(0.0), fZMax(0.0),
      fDx(0.0), fDy(0.0), fDz(0.0), fNx(0), fNy(0), fNz(0),
      fIsUniform(false), fLoadedSuccessfully(false)
{
    fLoadedSuccessfully = LoadFieldFile(filename);
    if (!fLoadedSuccessfully) {
        G4cerr << "DeVITOMagneticField: WARNING: Field initialization failed! All field values will return 0." << G4endl;
    }
}

DeVITOMagneticField::~DeVITOMagneticField() {}

G4bool DeVITOMagneticField::LoadFieldFile(const G4String& filename) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (!file) {
        G4cerr << "DeVITOMagneticField: Could not open file " << filename << G4endl;
        return false;
    }

    char line_buf[256];

    // Read the first line for nx, ny, nz
    if (!std::fgets(line_buf, sizeof(line_buf), file)) {
        G4cerr << "DeVITOMagneticField: Error reading first line of " << filename << G4endl;
        std::fclose(file);
        return false;
    }

    // NOTE: empirically, this field-map format stores the header
    // dimensions in REVERSE order: "NZ NY NX format_id", not
    // "NX NY NZ format_id".
    size_t nx = 0, ny = 0, nz = 0;
    int format_id = 0;
    if (std::sscanf(line_buf, "%zu %zu %zu %d", &nz, &ny, &nx, &format_id) < 3) {
        G4cerr << "DeVITOMagneticField: Error parsing grid dimensions from header: " << line_buf << G4endl;
        std::fclose(file);
        return false;
    }

    G4bool found_header_end = false;
    while (std::fgets(line_buf, sizeof(line_buf), file)) {
        // Trim leading space
        char* ptr = line_buf;
        while (*ptr && std::isspace(*ptr)) {
            ptr++;
        }
        if (*ptr == '0') {
            found_header_end = true;
            break;
        }
    }

    if (!found_header_end) {
        G4cerr << "DeVITOMagneticField: End of header '0' not found in " << filename << G4endl;
        std::fclose(file);
        return false;
    }

    size_t expected_total = nx * ny * nz;
    fNx = nx;
    fNy = ny;
    fNz = nz;

    fXCoords.assign(nx, 0.0);
    fYCoords.assign(ny, 0.0);
    fZCoords.assign(nz, 0.0);
    fBx.assign(expected_total, 0.0);
    fBy.assign(expected_total, 0.0);
    fBz.assign(expected_total, 0.0);

    G4cout << "DeVITOMagneticField: Loading " << expected_total << " points on a "
           << nx << " x " << ny << " x " << nz << " grid..." << G4endl;

    const double kCoordTol = 1e-3; // mm, tolerance for grid-order consistency check

    size_t m = 0;
    while (m < expected_total && std::fgets(line_buf, sizeof(line_buf), file)) {
        char* ptr = line_buf;
        while (*ptr && std::isspace(*ptr)) {
            ptr++;
        }
        if (*ptr == '\0') {
            continue; // Skip blank lines
        }

        char* next;
        double x = std::strtod(ptr, &next); if (ptr == next) continue; ptr = next;
        double y = std::strtod(ptr, &next); if (ptr == next) continue; ptr = next;
        double z = std::strtod(ptr, &next); if (ptr == next) continue; ptr = next;
        double bx = std::strtod(ptr, &next); if (ptr == next) continue; ptr = next;
        double by = std::strtod(ptr, &next); if (ptr == next) continue; ptr = next;
        double bz = std::strtod(ptr, &next); if (ptr == next) continue; // FIX 1: bz now guarded like the rest

        size_t i = m / (ny * nz);
        size_t j = (m / nz) % ny;
        size_t k = m % nz;


        if (j == 0 && k == 0) {
            fXCoords[i] = x;
        } else if (std::abs(fXCoords[i] - x) > kCoordTol) {
            G4cerr << "DeVITOMagneticField: ERROR: Unexpected X value at point " << m
                   << " (i=" << i << ", j=" << j << ", k=" << k << "). Expected X="
                   << fXCoords[i] << " but read X=" << x
                   << ". File is not ordered as X-slowest/Y/Z-fastest." << G4endl;
            std::fclose(file);
            return false;
        }

        if (i == 0 && k == 0) {
            fYCoords[j] = y;
        } else if (std::abs(fYCoords[j] - y) > kCoordTol) {
            G4cerr << "DeVITOMagneticField: ERROR: Unexpected Y value at point " << m
                   << " (i=" << i << ", j=" << j << ", k=" << k << "). Expected Y="
                   << fYCoords[j] << " but read Y=" << y
                   << ". File is not ordered as X-slowest/Y/Z-fastest." << G4endl;
            std::fclose(file);
            return false;
        }

        if (i == 0 && j == 0) {
            fZCoords[k] = z;
        } else if (std::abs(fZCoords[k] - z) > kCoordTol) {
            G4cerr << "DeVITOMagneticField: ERROR: Unexpected Z value at point " << m
                   << " (i=" << i << ", j=" << j << ", k=" << k << "). Expected Z="
                   << fZCoords[k] << " but read Z=" << z
                   << ". File is not ordered as X-slowest/Y/Z-fastest." << G4endl;
            std::fclose(file);
            return false;
        }

        fBx[m] = bx;
        fBy[m] = by;
        fBz[m] = bz;

        m++;
    }

    std::fclose(file);

    if (m < expected_total) {
        G4cerr << "DeVITOMagneticField: WARNING: Parsed only " << m << " points out of expected " << expected_total << G4endl;
        return false;
    }

    fXMin = fXCoords.front();
    fXMax = fXCoords.back();
    fYMin = fYCoords.front();
    fYMax = fYCoords.back();
    fZMin = fZCoords.front();
    fZMax = fZCoords.back();

    fDx = (nx > 1) ? (fXMax - fXMin) / (nx - 1) : 1.0;
    fDy = (ny > 1) ? (fYMax - fYMin) / (ny - 1) : 1.0;
    fDz = (nz > 1) ? (fZMax - fZMin) / (nz - 1) : 1.0;

    // Check uniformity of the grid coordinates
    fIsUniform = true;
    for (size_t i = 1; i < nx; ++i) {
        if (std::abs((fXCoords[i] - fXCoords[i-1]) - fDx) > 1e-4) { fIsUniform = false; break; }
    }
    for (size_t j = 1; j < ny; ++j) {
        if (std::abs((fYCoords[j] - fYCoords[j-1]) - fDy) > 1e-4) { fIsUniform = false; break; }
    }
    for (size_t k = 1; k < nz; ++k) {
        if (std::abs((fZCoords[k] - fZCoords[k-1]) - fDz) > 1e-4) { fIsUniform = false; break; }
    }

    G4cout << "DeVITOMagneticField: Field map loaded successfully." << G4endl;
    G4cout << "  X in [" << fXMin << ", " << fXMax << "] mm, step = " << fDx << " mm" << G4endl;
    G4cout << "  Y in [" << fYMin << ", " << fYMax << "] mm, step = " << fDy << " mm" << G4endl;
    G4cout << "  Z in [" << fZMin << ", " << fZMax << "] mm, step = " << fDz << " mm" << G4endl;
    G4cout << "  Uniform grid: " << (fIsUniform ? "Yes" : "No") << G4endl;

    return true;
}


// void DeVITOMagneticField::GetFieldValue(const G4double Point[4], G4double* Bfield) const
// {
//     Bfield[0] = 0.0;
//     Bfield[1] = 0.0;
//     Bfield[2] = 0.0;
// }

void DeVITOMagneticField::GetFieldValue(const G4double Point[4], G4double* Bfield) const {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;

    if (!fLoadedSuccessfully) {
        return;
    }

    // World-frame point
    double xw = Point[0];
    double yw = Point[1];
    double zw = Point[2];

    // --- Rotate point from world frame into the field's native (local) frame ---
    // This undoes a +90 deg rotation about Y that we want to APPLY to the field.
    // local = R(-90, Y) * world

    // Pointing in -z direction
    double x = -zw;
    double y =  yw;
    double z =  xw;

    // //Pointing in +z direction
    // double x = zw;
    // double y = yw;
    // double z = -xw;

    // Check bounds (in local/native frame)
    if (x < fXMin || x > fXMax || y < fYMin || y > fYMax || z < fZMin || z > fZMax) {
        return;
    }

    size_t i = 0, j = 0, k = 0;
    double u = 0.0, v = 0.0, w = 0.0;

    if (fIsUniform) {
        double x_idx_d = (x - fXMin) / fDx;
        double y_idx_d = (y - fYMin) / fDy;
        double z_idx_d = (z - fZMin) / fDz;

        i = static_cast<size_t>(std::floor(x_idx_d));
        j = static_cast<size_t>(std::floor(y_idx_d));
        k = static_cast<size_t>(std::floor(z_idx_d));

        if (i >= fNx - 1) i = fNx - 2;
        if (j >= fNy - 1) j = fNy - 2;
        if (k >= fNz - 1) k = fNz - 2;

        u = x_idx_d - i;
        v = y_idx_d - j;
        w = z_idx_d - k;
    } else {
        auto x_it = std::lower_bound(fXCoords.begin(), fXCoords.end(), x);
        i = std::distance(fXCoords.begin(), x_it);
        if (i > 0 && (i == fNx || fXCoords[i] > x)) i--;
        if (i >= fNx - 1) i = fNx - 2;
        u = (x - fXCoords[i]) / (fXCoords[i+1] - fXCoords[i]);

        auto y_it = std::lower_bound(fYCoords.begin(), fYCoords.end(), y);
        j = std::distance(fYCoords.begin(), y_it);
        if (j > 0 && (j == fNy || fYCoords[j] > y)) j--;
        if (j >= fNy - 1) j = fNy - 2;
        v = (y - fYCoords[j]) / (fYCoords[j+1] - fYCoords[j]);

        auto z_it = std::lower_bound(fZCoords.begin(), fZCoords.end(), z);
        k = std::distance(fZCoords.begin(), z_it);
        if (k > 0 && (k == fNz || fZCoords[k] > z)) k--;
        if (k >= fNz - 1) k = fNz - 2;
        w = (z - fZCoords[k]) / (fZCoords[k+1] - fZCoords[k]);
    }

    if (u < 0.0) u = 0.0; if (u > 1.0) u = 1.0;
    if (v < 0.0) v = 0.0; if (v > 1.0) v = 1.0;
    if (w < 0.0) w = 0.0; if (w > 1.0) w = 1.0;

    auto get_val = [this](const std::vector<double>& field, size_t idx_x, size_t idx_y, size_t idx_z) {
        return field[idx_x * (fNy * fNz) + idx_y * fNz + idx_z];
    };

    auto interpolate = [&](const std::vector<double>& field) {
        double c000 = get_val(field, i,     j,     k);
        double c100 = get_val(field, i + 1, j,     k);
        double c010 = get_val(field, i,     j + 1, k);
        double c110 = get_val(field, i + 1, j + 1, k);
        double c001 = get_val(field, i,     j,     k + 1);
        double c101 = get_val(field, i + 1, j,     k + 1);
        double c011 = get_val(field, i,     j + 1, k + 1);
        double c111 = get_val(field, i + 1, j + 1, k + 1);

        double c00 = c000 * (1.0 - u) + c100 * u;
        double c10 = c010 * (1.0 - u) + c110 * u;
        double c01 = c001 * (1.0 - u) + c101 * u;
        double c11 = c011 * (1.0 - u) + c111 * u;

        double c0 = c00 * (1.0 - v) + c10 * v;
        double c1 = c01 * (1.0 - v) + c11 * v;

        return c0 * (1.0 - w) + c1 * w;
    };

    double bx_local = interpolate(fBx);
    double by_local = interpolate(fBy);
    double bz_local = interpolate(fBz);

    // // // --- Rotate the field VECTOR back from local frame into world frame ---
    //Field pointing -z
    double bx_world =  bz_local;
    double by_world =  by_local;
    double bz_world = -bx_local;

    // // Field pointing +z
    // double bx_world = -bz_local;
    // double by_world = by_local;
    // double bz_world = bx_local;

    // Convert field values from Tesla (T) to Geant4 internal units
    Bfield[0] = bx_world * CLHEP::tesla;
    Bfield[1] = by_world * CLHEP::tesla;
    Bfield[2] = bz_world * CLHEP::tesla;
}