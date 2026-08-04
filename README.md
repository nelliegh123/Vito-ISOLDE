# Vito-ISOLDE
Sample simulations for the Vito-ISOLDE beamline. Part of the CERN summer student projects 2026.

### To be added:
A description of the project - what it is, what it is useful for and how it works (run annoying sim once and then just do calculations)

How to compile the project

### New Detector Geometry
Make new files _detectorGeometryNew.cc_ and _detectorGeometryNew.hh_ in src and include directories. Define the detector geometry (you can look at _detectorGeometryDeVITO_ for inspiration). 
In detectorConstruction.cc you include your new header file and call your new function. 
Run as normal. 

How to implement a new detector geometry (make separate file, implement geometry, add option in detectorConstruction to call that geometry)

How to implement a new sample (add option on detectorConstruction with a new sample, and modify make_macro.py to get the right shape of the emitted particles)

How to implement a new magnetic field (add option in detectorConstruction and make a new MagneticField.cc and hh file)

Comment on any specific processes: for example that particles are killed if they live for more than 1s and that backscatter is removed. 
