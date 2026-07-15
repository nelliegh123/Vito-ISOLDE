#!/bin/bash

numberOfParticles=10
macroFile=energy_angle_scan_full.mac
sampleType=liquid                     #Either solid or liquid
sampleThickness=0.1               #Thickness of solid sample (solid) or mica disc (liquid) in mm
liquidThickness=0.1               #Liquid sample thickness in mm
cd ..

make 

cd runScripts

python energy_angle_scan.py $numberOfParticles $sampleType $sampleThickness $liquidThickness

cd ..

./ISOLDE $macroFile $sampleType $sampleThickness $liquidThickness --gui

root "draw/draw_angle_energy.C($numberOfParticles)"