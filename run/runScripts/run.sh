#!/bin/bash
numberOfParticles=10  #Particles fired per step in energy/angle
sampleType=liquid     #solid_KCl, solid_MgO, liquid
sampleThickness=2.0   #thickness [mm] of sample (solid) or mica (liquid) 
liquidThickness=0.5   #thickness [mm] of liquid
sampleDiameter=12.0   #[mm] 
detector=default      #default, devito2023, devito2024
magField=devito       #default, devito
theta_start=0         #degrees
theta_stop=180        #degrees
n_steps=180
energy_min=0          #MeV
energy_max=5          #MeV
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


cp "$0" "$runDir/"
cd ../../build
cmake ..
make
cd ../run/runScripts
python make_macro.py $numberOfParticles $sampleType $sampleThickness $liquidThickness $sampleDiameter $theta_start $theta_stop $n_steps $energy_min $energy_max $n_energy_steps
cp run_commands.mac ../../build/
cd ../../build
./ISOLDE $sampleType $sampleThickness $liquidThickness $sampleDiameter $detector $magField --gui
mv output.root "$runDir/output.root"
echo "Run complete. Results in $runDir"