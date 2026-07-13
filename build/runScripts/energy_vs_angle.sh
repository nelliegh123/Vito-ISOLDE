#!/bin/bash

numberOfParticles=10
macroFile=energy_angle_scan_full.mac
sampleType=solid                       #Either solid or liquid
sampleThickness=0.1                   #Solid sample thickness in mm
cd ..

make 

cd runScripts

python energy_angle_scan.py $numberOfParticles 

cd ..

./ISOLDE $macroFile $sampleType $sampleThickness

root "draw/draw_angle_energy.C($numberOfParticles)"