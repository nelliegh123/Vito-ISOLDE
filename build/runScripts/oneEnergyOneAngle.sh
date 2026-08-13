#!/bin/bash

angle=0
energy=10

numberOfParticles=10  #Nr of particles fired per step

sampleType=solid_MgO         #Either solid_MgO, solid_KCl or liquid
sampleThickness=0.0        #Thickness of solid sample (solid) or mica disc (liquid) in mm
liquidThickness=0.01         #Liquid sample thickness in mm
sampleDiameter=12.0          #Sample diameter in mm

detector=devito2024    #Choose default,devito, devitoCircle2023, devitoCircle2024
magField=devito              #Choose vito, devito


# outputFile="oneEnergyOneAngle.root"

cd ..
cmake ..
make 
cd runScripts
python oneEnergyOneAngle.py $numberOfParticles $sampleType $sampleThickness $sampleDiameter $liquidThickness $angle $energy
cd ..
./ISOLDE $sampleType $sampleThickness $liquidThickness $sampleDiameter $detector $magField --gui
# root "draw/draw_oneEnergyOneAngle.C($numberOfParticles)"


