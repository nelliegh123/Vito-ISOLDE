# Vito-ISOLDE
Sample simulations for the Vito-ISOLDE beamline. Part of the CERN summer student projects 2026.

# To be added:
A description of the project - what it is, what it is useful for and how it works (run annoying sim once and then just do calculations)

### Compiling
How to compile the project

What to change for runs, for example set the magnetic field, the sample type, thickness and size. Write that if you set a solid sample to a certain diameter the particles will immediately be emitted uniformly from that diameter in make_macros.py. Same with volume distribution in liquid sample.  

### New Detector Geometry
- Make new files _detectorGeometryNew.cc_ and _detectorGeometryNew.hh_ in src and include directories. Define the detector geometry (you can look at _detectorGeometryDeVITO_ for inspiration). 
- In detectorConstruction.cc you include your new header file and call your new function.
- Update run.sh to use your new geometry. 
- Run as normal. 

### New Sample
How to implement a new sample (add option on detectorConstruction with a new sample, and modify make_macro.py to get the right shape of the emitted particles)

### New Magnetic Field
How to implement a new magnetic field (add option in detectorConstruction and make a new MagneticField.cc and hh file)

### Other Comments
Comment on any specific processes: for example that particles are killed if they live for more than 1s and that backscatter is removed. 
