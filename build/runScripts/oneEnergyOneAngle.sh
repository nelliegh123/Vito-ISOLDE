#!/bin/bash

angle=180
energy=10

numberOfParticles=10000
macroFile=oneEnergyOneAngle.mac
sampleType=liquid
sampleThickness=0.1
liquidThickness=0.1

outputFile="oneEnergyOneAngle.root"

cd ..
make 
cd runScripts
python oneEnergyOneAngle.py $numberOfParticles $sampleType $sampleThickness $liquidThickness $angle $energy
cd ..
./ISOLDE $macroFile $sampleType $sampleThickness $liquidThickness #--gui
root "draw/draw_oneEnergyOneAngle.C($numberOfParticles)"



