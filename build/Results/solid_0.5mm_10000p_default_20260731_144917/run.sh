#!/bin/bash
numberOfParticles=10000     #Nr of particles fired per step 
sampleType=solid            #Either solid or liquid
sampleThickness=0.5           #Thickness of solid sample (solid) or mica disc (liquid) in mm
liquidThickness=0.01        #Liquid sample thickness in mm
detector=default             #Choose default, DeVITO, 

runTag="${sampleType}_${sampleThickness}mm_${numberOfParticles}p_${detector}"
timestamp=$(date +%Y%m%d_%H%M%S)
runDir="$(cd .. && pwd)/Results/${runTag}_${timestamp}"
mkdir -p "$runDir"

cat > "${runDir}/params.txt" <<EOF
numberOfParticles=$numberOfParticles
sampleType=$sampleType
sampleThickness=$sampleThickness
liquidThickness=$liquidThickness
detector=$detector
timestamp=$timestamp
gitCommit=$(git rev-parse HEAD 2>/dev/null || echo "n/a")
EOF

python3 - <<'PYEOF' >> "${runDir}/params.txt"
import re

with open("make_macro.py") as f:
    src = f.read()
ns = {}
for line in re.findall(
    r'^\s*(theta_start\s*,\s*theta_stop\s*,\s*n_steps\s*=.*|'
    r'energy_min\s*,\s*energy_max\s*,\s*n_energy_steps\s*=.*)$',
    src, re.MULTILINE
):
    exec(line, ns)
for k in ["theta_start", "theta_stop", "n_steps",
          "energy_min", "energy_max", "n_energy_steps"]:
    print(f"{k}={ns[k]}")
PYEOF
cp "$0" "$runDir/"


cd ..
make 
cd runScripts
python make_macro.py $numberOfParticles $sampleType $sampleThickness $liquidThickness 
cd ..
./ISOLDE $sampleType $sampleThickness $liquidThickness $detector 

mv output.root "$runDir/output.root"
echo "Run complete. Results in $runDir"
