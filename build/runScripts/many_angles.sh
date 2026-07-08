#!/bin/bash

numberOfParticles=10
macroFile=angle_scan_full.mac0

energies=(1 3 5 7 9)

cd ..

make

for energy in "${energies[@]}"
do
    cd runScripts

    outputFile="rootFiles/Scan_${energy}MeV.root"
    python angle_python.py "$numberOfParticles" "$energy" "$outputFile"

    cd ..

    ./ISOLDE "$macroFile"
done

root "draw/draw_many_angles.C($numberOfParticles)"