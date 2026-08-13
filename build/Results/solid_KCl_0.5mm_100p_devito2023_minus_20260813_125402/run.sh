#!/bin/bash
numberOfParticles=100          #Nr of particles fired per step

sampleType=solid_KCl         #Either solid_MgO, solid_KCl or liquid
sampleThickness=0.5          #Thickness of solid sample (solid) or mica disc (liquid) in mm
# sampleThickness=${1:-2.0}
liquidThickness=0.01         ## OBS LIQUID need flipping due to flidded detector geom #Liquid sample thickness in mm
sampleDiameter=12.0          #Sample diameter in mm

detector=devito2023          #Choose default,devito2023, devito2024,
magField=devito              #Choose vito, devito

theta_start=0
theta_stop=180
n_steps=180

energy_min=0
energy_max=5
n_energy_steps=100

mag_field=minus



runTag="${sampleType}_${sampleThickness}mm_${numberOfParticles}p_${detector}_${mag_field}"
timestamp=$(date +%Y%m%d_%H%M%S)
runDir="$(cd .. && pwd)/Results/${runTag}_${timestamp}"
mkdir -p "$runDir"

cat > "${runDir}/params.txt" <<EOF
numberOfParticles=$numberOfParticles
sampleType=$sampleType
sampleThickness=$sampleThickness
liquidThickness=$liquidThickness
detector=$detector
theta_start=$theta_start
theta_stop=$theta_stop
n_steps=$n_steps
energy_min=$energy_min
energy_max=$energy_max
n_energy_steps=$n_energy_steps
mag_field=$mag_field
timestamp=$timestamp

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
cmake ..
make 
cd runScripts
python make_macro.py $numberOfParticles $sampleType $sampleThickness $liquidThickness $sampleDiameter $theta_start $theta_stop $n_steps $energy_min $energy_max $n_energy_steps
cd ..
./ISOLDE $sampleType $sampleThickness $liquidThickness $sampleDiameter $detector $magField #--gui

mv output.root "$runDir/output.root"
echo "Run complete. Results in $runDir"
