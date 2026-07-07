#!/bin/bash

numberOfParticles=100
macroFile=angle_scan_full.mac

cd ..

make 

cd runScripts

python angle_python.py $numberOfParticles 

cd ..

./ISOLDE $macroFile 

root draw/draw_angle.C