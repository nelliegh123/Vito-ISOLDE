#!/bin/bash
# set -e   # stop on first error instead of continuing with a broken state

numberOfParticles=10
sampleType=solid_KCl
sampleThickness=2.0
liquidThickness=0.01
sampleDiameter=12.0
detector=devito2023
magField=devito
theta_start=0
theta_stop=180
n_steps=180
energy_min=0
energy_max=5
n_energy_steps=100
mag_field=plus

runTag="${sampleType}_${sampleThickness}mm_${numberOfParticles}p_${detector}_${mag_field}"
timestamp=$(date +%Y%m%d_%H%M%S)
runDir="$(cd ../Results && pwd)/${runTag}_${timestamp}"
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
cd ../../build
cmake ..
make
cd ../run/runScripts
python make_macro.py $numberOfParticles $sampleType $sampleThickness $liquidThickness $sampleDiameter $theta_start $theta_stop $n_steps $energy_min $energy_max $n_energy_steps
cp run_commands.mac ../../build/
cd ../../build
./ISOLDE $sampleType $sampleThickness $liquidThickness $sampleDiameter $detector $magField #--gui
mv output.root "$runDir/output.root"
echo "Run complete. Results in $runDir"