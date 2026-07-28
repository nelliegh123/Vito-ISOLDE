#!/bin/bash

numberOfParticles=100
macroFile=energy_scan_full.mac
angle=45

outputFile="TotalAngleScan.root"

cd ..

make 

cd runScripts

python energy_scan.py $numberOfParticles $angle $outputFile

cd ..

./ISOLDE $macroFile 

root draw/draw.C