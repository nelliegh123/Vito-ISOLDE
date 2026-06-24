#include "VITOMagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <stdexcept>

VITOMagneticField::VITOMagneticField(const G4String& f1Axial, const G4String& f1Radial,
                                     const G4String& f2Axial, const G4String& f2Radial,
                                     G4bool symmetric)
    : fSymmetric(symmetric), fLoadedSuccessfully(true)
{
    if (!LoadFieldFile(f1Axial, fRegion1Axial)) {
        G4cerr << "VITOMagneticField: Error loading Region 1 Axial file: " << f1Axial << G4endl;
        fLoadedSuccessfully = false;
    }
    if (!LoadFieldFile(f1Radial, fRegion1Radial)) {
        G4cerr << "VITOMagneticField: Error loading Region 1 Radial file: " << f1Radial << G4endl;
        fLoadedSuccessfully = false;
    }

    // Load Region 2 files
    if (!LoadFieldFile(f2Axial, fRegion2Axial)) {
        G4cerr << "VITOMagneticField: Error loading Region 2 Axial file: " << f2Axial << G4endl;
        fLoadedSuccessfully = false;
    }
    if (!LoadFieldFile(f2Radial, fRegion2Radial)) {
        G4cerr << "VITOMagneticField: Error loading Region 2 Radial file: " << f2Radial << G4endl;
        fLoadedSuccessfully = false;
    }

    if (fLoadedSuccessfully) {
        G4cout << "VITOMagneticField: All field data files loaded successfully." << G4endl;
        G4cout << "Region 1: Z in [" << fRegion1Axial.z_min << ", " << fRegion1Axial.z_max << "] m, "
               << "R in [" << fRegion1Axial.r_min << ", " << fRegion1Axial.r_max << "] m." << G4endl;
        G4cout << "Region 2: Z in [" << fRegion2Axial.z_min << ", " << fRegion2Axial.z_max << "] m, "
               << "R in [" << fRegion2Axial.r_min << ", " << fRegion2Axial.r_max << "] m." << G4endl;
    } else {
        G4cerr << "VITOMagneticField: WARNING: Field initialization failed! All field values will return 0." << G4endl;
    }
}

VITOMagneticField::~VITOMagneticField() {}

G4bool VITOMagneticField::LoadFieldFile(const G4String& filename, GridData& grid) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        G4cerr << "VITOMagneticField: Could not open file " << filename << G4endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) {
        G4cerr << "VITOMagneticField: File is empty: " << filename << G4endl;
        return false;
    }
    std::string header_line = line;

    std::vector<std::string> header_tokens;
    std::stringstream header_ss(header_line);
    std::string token;
    while (std::getline(header_ss, token, '\t')) {
        // Strip trailing \r, spaces, or tabs
        while (!token.empty() && (token.back() == '\r' || token.back() == ' ' || token.back() == '\t')) {
            token.pop_back();
        }
        // Strip leading spaces or tabs
        size_t start_pos = 0;
        while (start_pos < token.size() && (token[start_pos] == ' ' || token[start_pos] == '\t')) {
            start_pos++;
        }
        if (start_pos > 0) {
            token = token.substr(start_pos);
        }
        if (!token.empty()) {
            header_tokens.push_back(token);
        }
    }

    std::vector<double> z_list;
    std::vector<std::vector<double>> values_grid;

    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        double z_val;
        if (!(ss >> z_val)) {
            // Not a numeric data line (e.g. comment/header), skip
            continue;
        }
        z_list.push_back(z_val);

        std::vector<double> row_vals;
        double val;
        while (ss >> val) {
            row_vals.push_back(val);
        }
        values_grid.push_back(row_vals);
    }

    if (values_grid.empty()) {
        G4cerr << "VITOMagneticField: No data rows parsed from " << filename << G4endl;
        return false;
    }

    size_t num_cols = values_grid[0].size();
    for (size_t i = 1; i < values_grid.size(); ++i) {
        if (values_grid[i].size() != num_cols) {
            values_grid[i].resize(num_cols, 0.0);
        }
    }

    // If tab splitting didn't yield enough tokens, fallback to splitting by whitespace
    if (header_tokens.size() < num_cols) {
        header_tokens.clear();
        std::stringstream header_ss2(header_line);
        std::string tok;
        while (header_ss2 >> tok) {
            header_tokens.push_back(tok);
        }
    }

    std::vector<double> r_list;
    // Determine R coordinates from the header tokens
    try {
        if (header_tokens.size() == num_cols) {
            // Header lists only R coordinates directly (no text label in column 1)
            for (const auto& t : header_tokens) {
                r_list.push_back(std::stod(t));
            }
        } else if (header_tokens.size() == num_cols + 1) {
            // First column is a text label like "Z(m)/R(m)-[Br(T)]"
            for (size_t i = 1; i < header_tokens.size(); ++i) {
                r_list.push_back(std::stod(header_tokens[i]));
            }
        } else {
            // Fallback heuristics: check if first token is numeric
            bool first_is_numeric = false;
            try {
                std::stod(header_tokens[0]);
                first_is_numeric = true;
            } catch (...) {}

            if (first_is_numeric && header_tokens.size() >= num_cols) {
                for (size_t i = 0; i < num_cols; ++i) {
                    r_list.push_back(std::stod(header_tokens[i]));
                }
            } else if (header_tokens.size() >= num_cols + 1) {
                for (size_t i = 1; i <= num_cols; ++i) {
                    r_list.push_back(std::stod(header_tokens[i]));
                }
            } else {
                G4cerr << "VITOMagneticField: Column count mismatch in header of " << filename 
                       << " (columns = " << num_cols << ", header tokens = " << header_tokens.size() << ")" << G4endl;
                return false;
            }
        }
    } catch (const std::exception& e) {
        G4cerr << "VITOMagneticField: Exception converting header token to double in " << filename 
               << ": " << e.what() << G4endl;
        return false;
    }

    grid.z_coords = std::move(z_list);
    grid.r_coords = std::move(r_list);
    grid.field_values = std::move(values_grid);

    grid.z_min = grid.z_coords.front();
    grid.z_max = grid.z_coords.back();
    grid.r_min = grid.r_coords.front();
    grid.r_max = grid.r_coords.back();

    size_t nz = grid.z_coords.size();
    size_t nr = grid.r_coords.size();

    // Check if the grid spacing is uniform
    grid.is_uniform = true;
    if (nz >= 2) {
        grid.z_step = (grid.z_max - grid.z_min) / (nz - 1);
        for (size_t i = 1; i < nz; ++i) {
            double step = grid.z_coords[i] - grid.z_coords[i-1];
            if (std::abs(step - grid.z_step) > 1e-6) {
                grid.is_uniform = false;
                break;
            }
        }
    }
    if (nr >= 2) {
        grid.r_step = (grid.r_max - grid.r_min) / (nr - 1);
        for (size_t i = 1; i < nr; ++i) {
            double step = grid.r_coords[i] - grid.r_coords[i-1];
            if (std::abs(step - grid.r_step) > 1e-6) {
                grid.is_uniform = false;
                break;
            }
        }
    }

    return true;
}

G4bool VITOMagneticField::Interpolate2D(const GridData& axial, const GridData& radial,
                                         G4double z, G4double r,
                                         G4double& Bz, G4double& Br) const
{
    const auto& z_coords = axial.z_coords;
    const auto& r_coords = axial.r_coords;

    // Check bounds
    if (z < z_coords.front() || z > z_coords.back() || r < r_coords.front() || r > r_coords.back()) {
        return false;
    }

    size_t nz = z_coords.size();
    size_t nr = r_coords.size();

    size_t i = 0;
    size_t j = 0;
    double u = 0.0;
    double v = 0.0;

    // If grid is uniform, compute indices in O(1)
    if (axial.is_uniform && radial.is_uniform) {
        double z_idx_d = (z - axial.z_min) / axial.z_step;
        double r_idx_d = (r - axial.r_min) / axial.r_step;

        i = static_cast<size_t>(std::floor(z_idx_d));
        j = static_cast<size_t>(std::floor(r_idx_d));

        if (i >= nz - 1) i = nz - 2;
        if (j >= nr - 1) j = nr - 2;

        u = z_idx_d - i;
        v = r_idx_d - j;
    } else {
        // Fallback to binary search for non-uniform grids
        auto z_it = std::lower_bound(z_coords.begin(), z_coords.end(), z);
        i = std::distance(z_coords.begin(), z_it);
        if (i > 0 && (i == nz || z_coords[i] > z)) i--;
        if (i >= nz - 1) i = nz - 2;

        auto r_it = std::lower_bound(r_coords.begin(), r_coords.end(), r);
        j = std::distance(r_coords.begin(), r_it);
        if (j > 0 && (j == nr || r_coords[j] > r)) j--;
        if (j >= nr - 1) j = nr - 2;

        u = (z - z_coords[i]) / (z_coords[i+1] - z_coords[i]);
        v = (r - r_coords[j]) / (r_coords[j+1] - r_coords[j]);
    }

    // Clamp fractional positions to [0, 1] to prevent floating point inaccuracies
    if (u < 0.0) u = 0.0; if (u > 1.0) u = 1.0;
    if (v < 0.0) v = 0.0; if (v > 1.0) v = 1.0;

    // Bilinear interpolation for Bz
    double bz00 = axial.field_values[i][j];
    double bz10 = axial.field_values[i+1][j];
    double bz01 = axial.field_values[i][j+1];
    double bz11 = axial.field_values[i+1][j+1];

    Bz = (1.0 - u) * (1.0 - v) * bz00 +
         u * (1.0 - v) * bz10 +
         (1.0 - u) * v * bz01 +
         u * v * bz11;

    // Bilinear interpolation for Br
    double br00 = radial.field_values[i][j];
    double br10 = radial.field_values[i+1][j];
    double br01 = radial.field_values[i][j+1];
    double br11 = radial.field_values[i+1][j+1];

    Br = (1.0 - u) * (1.0 - v) * br00 +
         u * (1.0 - v) * br10 +
         (1.0 - u) * v * br01 +
         u * v * br11;

    return true;
}

void VITOMagneticField::GetFieldValue(const G4double Point[4], G4double* Bfield) const {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;

    if (!fLoadedSuccessfully) {
        return;
    }

    // Convert coordinates from Geant4 internal units (mm) to meters
    double x_m = Point[0] / CLHEP::m;
    double y_m = Point[1] / CLHEP::m;
    double z_m = Point[2] / CLHEP::m;

    double r_m = std::sqrt(x_m * x_m + y_m * y_m);
    
    // Support symmetric lookup about Z = 0 if enabled
    double abs_z_m = fSymmetric ? std::abs(z_m) : z_m;

    double Bz_val = 0.0;
    double Br_val = 0.0;
    bool found = false;

    // Check which region the point falls into
    if (abs_z_m >= fRegion1Axial.z_min && abs_z_m <= fRegion1Axial.z_max &&
        r_m >= fRegion1Axial.r_min && r_m <= fRegion1Axial.r_max)
    {
        found = Interpolate2D(fRegion1Axial, fRegion1Radial, abs_z_m, r_m, Bz_val, Br_val);
    }
    else if (abs_z_m >= fRegion2Axial.z_min && abs_z_m <= fRegion2Axial.z_max &&
             r_m >= fRegion2Axial.r_min && r_m <= fRegion2Axial.r_max)
    {
        found = Interpolate2D(fRegion2Axial, fRegion2Radial, abs_z_m, r_m, Bz_val, Br_val);
    }

    if (found) {
        // Convert field values from Tesla (T) to Geant4 internal units
        double Bz_internal = Bz_val * CLHEP::tesla;
        double Br_internal = Br_val * CLHEP::tesla;

        if (fSymmetric && z_m < 0.0) {
            // Radial field is antisymmetric about the Z = 0 plane
            Br_internal = -Br_internal;
        }

        // Project radial field onto Cartesian coordinates (x, y)
        if (r_m > 0.0) {
            Bfield[0] = Br_internal * (x_m / r_m);
            Bfield[1] = Br_internal * (y_m / r_m);
        } else {
            Bfield[0] = 0.0;
            Bfield[1] = 0.0;
        }
        Bfield[2] = Bz_internal;
    }
}
