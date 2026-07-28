#!/bin/bash

numberOfParticles=10           #Nr of particles fired per step 
sampleType=solid                #Either solid or liquid
sampleThickness=0.1              #Thickness of solid sample (solid) or mica disc (liquid) in mm
liquidThickness=0.01                #Liquid sample thickness in mm

runTag="${sampleType}_${sampleThickness}mm_${numberOfParticles}p"
timestamp=$(date +%Y%m%d_%H%M%S)

runDir="$(cd .. && pwd)/runs/${runTag}_${timestamp}"
mkdir -p "$runDir"

cat > "${runDir}/params.txt" <<EOF
numberOfParticles=$numberOfParticles
sampleType=$sampleType
sampleThickness=$sampleThickness
liquidThickness=$liquidThickness
timestamp=$timestamp
# gitCommit=$(git rev-parse HEAD 2>/dev/null || echo "n/a")
EOF

cp "$0" "$runDir/"
# cp "$macroFile" "$runDir/" 2>/dev/null


cd ..
make 
cd runScripts
python energy_angle_scan.py $numberOfParticles $sampleType $sampleThickness $liquidThickness
cd ..
./ISOLDE $sampleType $sampleThickness $liquidThickness \
    2>&1 | tee "${runDir}/stdout.log"

mv output.root "$runDir/output.root"
echo "Run complete. Results in $runDir"
