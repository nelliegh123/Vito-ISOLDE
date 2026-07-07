import numpy as np
import sys 

theta_start, theta_stop, n_steps = 0, 180, 46
energy_min, energy_max, n_energy_steps = 1, 10, 19

if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1


with open("energy_angle_scan_full.mac", "w") as f:
    f.write("/gps/particle e-\n")
    f.write("/gps/pos/type Plane\n")
    f.write("/gps/pos/shape Circle\n")
    f.write("/gps/pos/radius 0.5 cm\n")
    f.write("/gps/pos/centre 0 0 -0.5 mm\n")
    f.write("/gps/ene/type Mono\n")
    f.write("/analysis/openFile TotalAngleScan.root\n")

    for theta_deg in np.linspace(theta_start, theta_stop, n_steps):
        for energy in np.linspace(energy_min, energy_max, n_energy_steps):

            theta_rad = np.radians(theta_deg)
            y, z = np.sin(theta_rad), np.cos(theta_rad)
            f.write(f"/gps/direction 0 {y:.6f} {z:.6f}\n")

            f.write(f"/gps/ene/mono {energy:g} MeV\n")
            f.write(f"/run/beamOn {numberOfParticles:g}\n")

    f.write("/analysis/write\n")
    f.write("/analysis/closeFile\n")