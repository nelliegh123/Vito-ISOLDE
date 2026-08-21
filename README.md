# Vito-ISOLDE
Sample simulations for the Vito-ISOLDE beamline. Part of the CERN summer student projects 2026.

# Purpose:
The purpose of this code is to easily be able to calculate the experimental asymmetry parameter for $\beta$-NMR using different samples, detector setups and magnetic field in Geant4. The code is structured in such a way that you implement a detector geometry, a magnetic field and a sample. Then, an energy and angle scan is perfomed where electrons are fired at angles from $\theta=0$ (in the direction of the beam) to $\theta=180$ (opposite to the beam). After the angle and energy scan has been run, the asymmetry parameter can be calculated for a specific isotope, with a $\beta$ energy distribution, an initial asymmetry $A$ and polarization $P$. 


### Compiling
How to compile the project

What to change for runs, for example set the magnetic field, the sample type, thickness and size. Write that if you set a solid sample to a certain diameter the particles will immediately be emitted uniformly from that diameter in make_macros.py. Same with volume distribution in liquid sample.  

### New Detector Geometry
- Make new files _detectorGeometryNew.cc_ and _detectorGeometryNew.hh_ in src and include directories. Define the detector geometry as normal (you can look at _detectorGeometryDeVITO_ for inspiration). 
- In detectorConstruction.cc you include your new header file. You then add an instance and call your new function. It may look like:
```cpp
else if (fDetector == "devito") {
        detectorGeom = std::make_unique<DetectorGeometryDeVito>(fDetector);
}```
- Update run.sh to use your new geometry. 
- Run as normal. 

### New Sample
How to implement a new sample (add option on detectorConstruction with a new sample, and modify make_macro.py to get the right shape of the emitted particles)

### New Magnetic Field
How to implement a new magnetic field (add option in detectorConstruction and make a new MagneticField.cc and hh file)

### Calculating the Asymmetry
- include beta energy spectra in EnergySpectras
- Read this file into calculateAsym.py
- Set the initial asymmetry $A$ and the polarization $P$. Note that the angle $\theta$ in the equation $W(\theta) = 1+\frac{v}{c}PA\cos(\theta)$ is defined as the angle relative to the polarization axis. If your polarization is pointing in the -z direction, you must define the angle as 180-$\theta$.

### Other Comments
Comment on any specific processes: for example that particles are killed if they live for more than 1s and that backscatter is removed. 
