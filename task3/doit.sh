#!/bin/sh
~/Documents/iac/lab0-devtools/tools/attach_usb.sh
#!/usr/bin/env bash
set -euo pipefail

# cleanup
rm -rf obj_dir
rm -f *.vcd

verilator -Wall -sv --cc --trace \
  -Mdir obj_dir \
  --top-module f1_light \
  clktick.sv f1_fsm.sv f1_light.sv \
  --exe f1_light_tb.cpp

make -j"$(nproc)" -C obj_dir -f Vf1_light.mk Vf1_light

# 3) Run the sim
echo "Running simulation"
./obj_dir/Vf1_light
echo "Simulation completed"
