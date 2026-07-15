import numpy as np
import sys 

if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1
sampleType = str(sys.argv[2])
sampleThickness = float(sys.argv[3])
liquidThickness = float(sys.argv[4])
angle = float(sys.argv[5])
energy = float(sys.argv[6])


with open("oneEnergyOneAngle.mac", "w") as f:
    if sampleType=="solid":
        f.write("/gps/pos/type Plane\n")
        f.write(f"/gps/pos/centre 0 0 {-sampleThickness/2.0:.6f} mm\n")
        f.write("/gps/pos/shape Circle\n")

    elif sampleType=="liquid":
        z_center = -(sampleThickness / 2.0 + liquidThickness / 2.0)
        theta = np.radians(45.0)
        y_new = -z_center * np.sin(theta)
        z_new = z_center * np.cos(theta)
        # y_new = z_center * np.sin(theta) #Not sure which one to use
        # z_new = -z_center * np.cos(theta)

        f.write("/gps/pos/type Volume\n")
        f.write("/gps/pos/shape Cylinder\n")
        # f.write(f"/gps/pos/centre 0 0 {z_center:.6f} mm\n")

        f.write(f"/gps/pos/halfz {liquidThickness/2.0:.6f} mm\n")
        f.write(f"/gps/pos/centre 0 {y_new:.6f} {z_new:.6f} mm\n")

        f.write("/gps/pos/rot1 1 0 0\n")
        f.write("/gps/pos/rot2 0 0.707107 0.707107\n")
        # f.write("/gps/pos/confine liquidEMIM\n")
    
    else: 
        print("---------------Wrong sample-type, use liquid or solid!!------------")

    f.write("/gps/particle e-\n")
    f.write("/gps/pos/radius 0.5 cm\n")
    f.write("/gps/ene/type Mono\n")
    f.write("/analysis/openFile TotalAngleScan.root\n")


    theta_rad = np.radians(angle)
    y, z = np.sin(theta_rad), np.cos(theta_rad)
    f.write(f"/gps/direction 0 {y:.6f} {z:.6f}\n")

    f.write(f"/gps/ene/mono {energy:g} MeV\n")
    f.write(f"/run/beamOn {numberOfParticles:g}\n")

    f.write("/analysis/write\n")
    f.write("/analysis/closeFile\n")

