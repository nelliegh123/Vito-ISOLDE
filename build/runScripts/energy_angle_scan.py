import numpy as np
import sys 

theta_start, theta_stop, n_steps = 0, 180, 46
energy_min, energy_max, n_energy_steps = 1, 10, 19

if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1
sampleType = str(sys.argv[2])
sampleThickness = float(sys.argv[3])
liquidThickness = float(sys.argv[4])


with open("energy_angle_scan_full.mac", "w") as f:
    if sampleType=="solid":
        f.write("/gps/pos/type Plane\n")
        f.write(f"/gps/pos/centre 0 0 {-sampleThickness/2.0:.6f} mm\n")
        f.write("/gps/pos/shape Circle\n")

    elif sampleType=="liquid":
        z_center = -(sampleThickness / 2.0 + liquidThickness / 2.0)
        f.write("/gps/pos/type Volume\n")
        f.write("/gps/pos/shape Cylinder\n")
        f.write(f"/gps/pos/centre 0 0 {z_center:.6f} mm\n")
        f.write(f"/gps/pos/halfz {liquidThickness/2.0:.6f} mm\n")
    
    else: 
        print("---------------Wrong sample-type, use liquid or solid!!------------")

    f.write("/gps/particle e-\n")
    f.write("/gps/pos/radius 0.5 cm\n")
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

