#!/bin/bash

numberOfParticles=100
macroFile=angle_scan_full.mac

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

root draw/draw_many_angles.C