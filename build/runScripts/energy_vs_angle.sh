#!/bin/bash

numberOfParticles=100
macroFile=energy_angle_scan_full.mac

cd ..

make 

cd runScripts

python energy_angle_scan.py $numberOfParticles 

cd ..

./ISOLDE $macroFile 

root "draw/draw_angle_energy.C($numberOfParticles)"