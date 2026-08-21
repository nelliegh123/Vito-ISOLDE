# Purpose:
The purpose of this code is to easily be able to calculate the experimental asymmetry parameter for $\beta$-NMR using different samples, detector setups and magnetic field in Geant4. The code is structured in such a way that you implement a detector geometry, a magnetic field, and a sample. Then, an energy and angle scan is performed where electrons are fired at angles from $\theta=0$ to $\theta=180$, and a chosen range of energies (for example 0-14 MeV). This produces a ROOT file _output.root_ with the simulated hits in the front and rear detector. After the angle and energy scan has been run, the code in the directory git@github.com:nelliegh123/VITO-Analysis.git can be used to calculate the asymmetry parameter for a specific nuclei. 


## Installing and Compiling
Running the project requires Geant4, CMake, GNU Make, and Python 3. 

1. Clone the repository from GitHub "git@github.com:nelliegh123/Vito-ISOLDE.git"
2. Go into the runScripts directory
```bash
   cd build/runScripts
   code run.sh
```

What to change for runs, for example set the magnetic field, the sample type, thickness and size. Write that if you set a solid sample to a certain diameter the particles will immediately be emitted uniformly from that diameter in make_macros.py. Same with volume distribution in liquid sample.  

## New Detector Geometry
1. Make new files _detectorGeometryNew.cc_ and _detectorGeometryNew.hh_ in src and include directories. Define the detector geometry as normal (you can look at _detectorGeometryDeVITO_ for inspiration). 
2. In detectorConstruction.cc you include your new header file as _#include "detectorGeometryNew.hh"_. Scoll down to the section _Defining Detector_ and call your new function. It may look like:
```cpp
else if (fDetector == "new") {
        detectorGeom = std::make_unique<DetectorGeometryNew>(fDetector);
}
```
3, In _run.sh_ you can now set
  ```bash
  detector=new
  ```
  and you new geometry should be used.   

## New Sample
1. In _detecorConstruction.cc_ you add a new instance
   ```cpp
   else if(fSampleType == "new_sample" && fSampleThickness != 0.0) {
        G4Material *solidSampleMat = nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
        G4Tubs *solidSampleCylinder = new G4Tubs("solidSampleCylinder", 0., fSampleDiameter/2.0*mm,
        fSampleThickness/2.0*mm, 0.*deg, 360.*deg);
        G4LogicalVolume *logicSampleCylinder = new G4LogicalVolume(solidSampleCylinder, solidSampleMat, 
                                                                "logicSampleCylinder");
        G4VPhysicalVolume *solidCylinder = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicSampleCylinder, 
                                                                "solidCylinder", logicWorld, false, 0, true);                        
    }

   ```
2. In make_macro.py you add
```python
elif sampleType=="new_sample":
            f.write("/gps/pos/type Plane\n")
            f.write(f"/gps/pos/centre 0 0 {sampleThickness/2.0:.6f} mm\n")
            f.write("/gps/pos/shape Circle\n")
```

How to implement a new sample (add option on detectorConstruction with a new sample, and modify make_macro.py to get the right shape of the emitted particles)

## New Magnetic Field
How to implement a new magnetic field (add option in detectorConstruction and make a new MagneticField.cc and hh file)

### Calculating the Asymmetry
- include beta energy spectra in EnergySpectras
- Read this file into calculateAsym.py
- Set the initial asymmetry $A$ and the polarization $P$. Note that the angle $\theta$ in the equation $W(\theta) = 1+\frac{v}{c}PA\cos(\theta)$ is defined as the angle relative to the polarization axis. If your polarization is pointing in the -z direction, you must define the angle as 180-$\theta$.

## Other Comments
Comment on any specific processes: for example that particles are killed if they live for more than 1s and that backscatter is removed. 
