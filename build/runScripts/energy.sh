#!/bin/bash

numberOfParticles=1000
macroFile=energy_scan_full.mac

cd ..

make 

python energy_scan.py $numberOfParticles  

./ISOLDE $macroFile 

root draw/draw.C