# Purpose:
The purpose of this code is to easily be able to calculate the experimental asymmetry for $\beta$-NMR using different samples, detector setups and magnetic field in Geant4. The code is structured in such a way that you implement a detector geometry, a magnetic field, and a sample. Then, an energy and angle scan is performed where electrons are fired at angles from $\theta=0$ to $\theta=180$, and a chosen range of energies (for example 0-14 MeV). This produces a ROOT file _output.root_ with the simulated hits in the front and rear detector. After the angle and energy scan has been run, the code in the directory "nelliegh123/VITO-Analysis.git" can be used to calculate the asymmetry parameter for a specific nuclei. 


## Installing and Compiling
Running the project requires Geant4, CMake, GNU Make, and Python 3. Running the program is done in the following way:

1. Clone the repository from GitHub "nelliegh123/Vito-ISOLDE.git"'
2. Make a build-folder
```bash
   mkdir build
``` 
4. Go into the run/runScripts directory, and open the file _run.sh_. 
```bash
   cd run/runScripts
   code run.sh (or vim run.sh, or whatever you prefer)
```
3.  To make compiling easy, _run.sh_ runs "cmake .." and "make", before calling the python file _make_macro.py_ with your run parameters as arguments. This script creates a macro _run_commands.mac_, which contains all the information about how the particles will be fired in the simulations (how many, at what angles and energies etc). So all you have to do is set your run-parameters:
```bash
   numberOfParticles=1          #Nr of particles fired per step
   sampleType=solid_KCl         #solid_MgO, solid_KCl, liquid
   sampleThickness=2.0          #Thickness of solid sample (solid) or mica disc (liquid) in mm
   liquidThickness=0.01         #Liquid sample thickness in mm
   sampleDiameter=20.0          #Sample diameter in mm
   detector=devito2024          #Choose default, devito2023, devito2024,
   magField=devito              #Choose vito, devito
   theta_start=0                #Degrees
   theta_stop=180               #Degrees
   n_steps=180               
   energy_min=0                 #MeV 
   energy_max=5                 #MeV 
   n_energy_steps=100
```
4. Make the file executable, and run it as:
```bash
   chmod +x run.sh
   ./run.sh
```
5. Voilà! You now have a ROOT file named output.root located in a directory named based on your run parameters, inside run/Results.


## New Sample
If you want to define a new sample type, you do it in the following way:
1. Add a new instance in _detecorConstruction.cc_ 
   ```cpp
   else if(fSampleType == "new_sample" && fSampleThickness != 0.0) {
        G4Material *solidSampleMat = nist->FindOrBuildMaterial("NEW_MATERIAL");
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
3. In _run.h_ you should now be able to set "sampleType=new_sample"


## New Detector Geometry
If you want to implement a new detector geometry, you should do it in the following way:
1. Make new source and header files _detectorGeometryNew.cc_ and _detectorGeometryNew.hh_ in src and include directories. Define the detector geometry as normal (you can look at _detectorGeometryDeVITO_ for inspiration). Your new class should inherit from detectorGeometryDefault
```cpp
   class DetectorGeometryNew : public DetectorGeometryDefault
```
2. In detectorConstruction.cc you include your new header file as _#include "detectorGeometryNew.hh"_. Scoll down to the section _Defining Detector_ and call your new function. It may look like:
```cpp
else if (fDetector == "new") {
        detectorGeom = std::make_unique<DetectorGeometryNew>(fDetector);
}
```
3. In _run.sh_ you can now set
  ```bash
  detector=new
  ```
  and you new geometry should be used.   


## New Magnetic Field
If you want to implement a new magnetic field, you do:
1. Make new source and header files _NewMagneticField.cc_ and _NewMagneticField.hh_ in src and include directories. Here you include information such as interpolation of the field values.
2. Upload your .txt field map to build/Field
3. In _detectorConstruction.cc_, scroll down to the bottom and under the section _Activating the Magnetic Field_ you add
```cpp   
   else if (fMagField == "new") {
           G4MagneticField* magField = new NewMagneticField("../run/Field/Field-Map_New.txt");
           fField.Put(magField);
           G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
           pFieldMgr->SetDetectorField(fField.Get());
           pFieldMgr->CreateChordFinder(fField.Get());
       }
 ```
4. In _run.h_ you should now be able to set "magField=new"

## Analysis
Calculating the asymmetry parameter can be done in run/analysis/analysis.py. For an in-detail description of how this works, check out the repository VITO-Analysis.

## Other Comments
- The file _Field-Map_DeVITO.txt_ in run/Fields is really large. If you do not need it, consider deleting it.
- The ROOT file _output.root_ contains all the hits registered in the front and rear detector in the columns _Energy_, _Angle_ and _Detector ID_ (0 for front and 1 for rear). In the simulation, if an event runs for more than 1 second (usually because it gets stuck in the magnetic field), it is automatically killed. The ROOT file stores information about the removed events in the columns _rmPosX_, _rmPosY_, _rmPosZ_, _rmAngle_, and _rmEnergy_.
- A detector hit is defined as coming from primary electrons. Backscattering is removed by checking if this is a particles first interaction with a detector. If not, the particle is killed. 
- If a particle lives for more than one second, it is killed.
- To turn off the magnetic field. or change direction, you need to go into the file itself and change it. Setting "mag_field=plus" in run.sh only affects the name of the directory that contains the output file. 
