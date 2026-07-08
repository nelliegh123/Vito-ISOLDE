#!/bin/bash

numberOfParticles=1000
macroFile=energy_scan_full.mac

angles=(0 40 80 120 160)

cd ..

make

for angle in "${angles[@]}"
do
    cd runScripts

    outputFile="rootFiles/Scan_${angle}Degrees.root"
    python energy_scan.py "$numberOfParticles" "$angle" "$outputFile"

    cd ..

    ./ISOLDE "$macroFile"
done

root draw/draw_many_energies.C