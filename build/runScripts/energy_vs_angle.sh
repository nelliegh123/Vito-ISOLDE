#!/bin/bash

numberOfParticles=10000        #Nr of particles fired per step 
macroFile=energy_angle_scan_full.mac
sampleType=solid                #Either solid or liquid
sampleThickness=0.1              #Thickness of solid sample (solid) or mica disc (liquid) in mm
liquidThickness=0.01                #Liquid sample thickness in mm
#outputFile="output_${sampleType}_${sampleThickness}mm_${numberOfParticles}p.root"

cd ..
make 
cd runScripts
python energy_angle_scan.py $numberOfParticles $sampleType $sampleThickness $liquidThickness
cd ..
./ISOLDE $macroFile $sampleType $sampleThickness $liquidThickness #$outputFile
# python "draw/draw_from_TTree.py"
# python "runScripts/calculateAsym.py"