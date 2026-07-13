import numpy as np
import sys

energy_start, energy_stop, n_steps = 1, 1, 19

if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1

angle=int(sys.argv[2])
outputFile=str(sys.argv[3])

with open("energy_scan_full.mac", "w") as f:
    f.write("/gps/particle e-\n")
    f.write("/gps/pos/type Plane\n")
    f.write("/gps/pos/shape Circle\n")
    f.write("/gps/pos/radius 0.5 cm\n")
    f.write("/gps/pos/centre 0 0 -0.5 mm\n")
    f.write("/gps/ene/type Mono\n")
    f.write(f"/analysis/openFile {outputFile}\n")
    f.write("/gps/direction 0 0 1\n")

    for energy in np.linspace(energy_start, energy_stop, n_steps):
        theta_rad = np.radians(angle)
        y, z = np.sin(theta_rad), np.cos(theta_rad)
        f.write(f"/gps/direction 0 {y:.6f} {z:.6f}\n")

        f.write(f"/gps/ene/mono {energy:g} MeV\n")
        f.write(f"/run/beamOn {numberOfParticles:g}\n")

    f.write("/analysis/write\n")
    f.write("/analysis/closeFile\n")