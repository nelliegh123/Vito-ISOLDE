import numpy as np
import sys 

if len(sys.argv) > 1:
    numberOfParticles = int(sys.argv[1])
else:
    numberOfParticles = 1
sampleType = str(sys.argv[2])
sampleThickness = float(sys.argv[3])
liquidThickness = float(sys.argv[4])
sampleDiameter = float(sys.argv[5])
theta_start = int(sys.argv[6])
theta_stop = int(sys.argv[7])
n_steps = int(sys.argv[8])
energy_min = int(sys.argv[9])
energy_max = int(sys.argv[10])
n_energy_steps = int(sys.argv[11])

with open("run_commands.mac", "w") as f:
    if sampleType=="solid_MgO":
        f.write("/gps/pos/type Plane\n")
        f.write(f"/gps/pos/centre 0 0 {sampleThickness/2.0:.6f} mm\n")
        f.write("/gps/pos/shape Circle\n")

    elif sampleType=="solid_KCl":
            f.write("/gps/pos/type Plane\n")
            f.write(f"/gps/pos/centre 0 0 {sampleThickness/2.0:.6f} mm\n")
            f.write("/gps/pos/shape Circle\n")

    elif sampleType=="liquid":
        z_center = -(sampleThickness / 2.0 + liquidThickness / 2.0)
        theta = np.radians(45.0)
        y_new = -z_center * np.sin(theta)
        z_new = z_center * np.cos(theta)

        f.write("/gps/pos/type Volume\n")
        f.write("/gps/pos/shape Cylinder\n")
        f.write(f"/gps/pos/centre 0 {y_new:.6f} {z_new:.6f} mm\n")  

        f.write(f"/gps/pos/halfz {liquidThickness/2.0:.6f} mm\n")

        f.write("/gps/pos/rot1 1 0 0\n")
        f.write("/gps/pos/rot2 0 0.707107 0.707107\n")
    
    else:
        raise ValueError("Wrong sample-type, use liquid or solid!")

    f.write("/gps/particle e-\n")
    f.write(f"/gps/pos/radius {sampleDiameter/2.0:.6f} mm\n")
    f.write("/gps/ene/type Mono\n")
    f.write("/analysis/openFile output.root\n")


    f.write("/run/printProgress 100000\n")


    f.write("/mygen/scanMode true\n")
    f.write(f"/mygen/numParticlesPerPoint {numberOfParticles}\n")
    f.write(f"/mygen/nAngleSteps {n_steps}\n")
    f.write(f"/mygen/nEnergySteps {n_energy_steps}\n")
    f.write(f"/mygen/thetaMin {theta_start}\n")
    f.write(f"/mygen/thetaMax {theta_stop}\n")
    f.write(f"/mygen/energyMin {energy_min}\n")
    f.write(f"/mygen/energyMax {energy_max}\n")

    total_events = numberOfParticles * n_steps * n_energy_steps
    f.write(f"/run/beamOn {total_events}\n")

    f.write("/analysis/write\n")
    f.write("/analysis/closeFile\n")

