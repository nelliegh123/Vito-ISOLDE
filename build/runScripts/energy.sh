#!/bin/bash

numberOfParticles=1
macroFile=angle_scan.mac

cd .. && make && python energy_angle_scan.py $numberOfParticles  && ./ISOLDE $macroFile && root draw/draw_angle_energy.C