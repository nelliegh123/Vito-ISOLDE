#!/bin/bash

numberOfParticles=100
macroFile=energy_scan_full.mac

cd ..

make 

cd runScripts

python energy_scan.py $numberOfParticles 

cd ..

./ISOLDE $macroFile 

root draw/draw.C