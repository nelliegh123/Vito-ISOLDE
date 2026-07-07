#!/bin/bash

numberOfParticles=10
macroFile=angle_scan_full.mac

cd ..

make 

python angle_python.py $numberOfParticles  

./ISOLDE $macroFile 

root draw/draw_angle.C