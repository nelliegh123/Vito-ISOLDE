import numpy as np
import sys 
if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1
sampleType = str(sys.argv[2])
sampleThickness = float(sys.argv[3])
sampleDiameter = float(sys.argv[4])
liquidThickness = float(sys.argv[5])
angle = float(sys.argv[6])
energy = float(sys.argv[7])

with open("run_commands.mac", "w") as f:
    if sampleType == "solid_MgO" or sampleType == "solid_KCl":
        f.write("/gps/pos/type Plane\n")
        f.write(f"/gps/pos/centre 0 0 -{sampleThickness/2.0:.6f} mm\n")
        f.write("/gps/pos/shape Circle\n")

    elif sampleType=="liquid":
        z_center = -(sampleThickness / 2.0 + liquidThickness / 2.0)
        theta = np.radians(45.0)
        y_new = -z_center * np.sin(theta)
        z_new = z_center * np.cos(theta)
        f.write("/gps/pos/type Volume\n")
        f.write("/gps/pos/shape Cylinder\n")
        f.write(f"/gps/pos/halfz {liquidThickness/2.0:.6f} mm\n")
        f.write(f"/gps/pos/centre 0 {y_new:.6f} {z_new:.6f} mm\n")
        f.write("/gps/pos/rot1 1 0 0\n")
        f.write("/gps/pos/rot2 0 0.707107 0.707107\n")

    else: 
        raise ValueError("Wrong sample-type, use liquid or solid!")

    f.write("/gps/particle e-\n")
    f.write(f"/gps/pos/radius {sampleDiameter/2.0:.6f} mm\n")
    f.write("/gps/ene/type Mono\n")
    f.write("/analysis/openFile output.root\n")

    # --- use /mygen/ instead of a fixed /gps/direction and /gps/ene/mono ---
    f.write("/mygen/scanMode true\n")
    f.write(f"/mygen/numParticlesPerPoint {numberOfParticles}\n")
    f.write("/mygen/nAngleSteps 1\n")
    f.write("/mygen/nEnergySteps 1\n")
    f.write(f"/mygen/thetaMin {angle:g}\n")
    f.write(f"/mygen/thetaMax {angle:g}\n")
    f.write(f"/mygen/energyMin {energy:g}\n")
    f.write(f"/mygen/energyMax {energy:g}\n")

    f.write(f"/run/beamOn {numberOfParticles}\n")
    f.write("/vis/scene/add/axes 0 0 0 0.1 m \n")
    f.write("/analysis/write\n")
    f.write("/analysis/closeFile\n")