#!/usr/bin/env bash
set -Eeuo pipefail
trap 'echo "ERROR at line $LINENO"; exit 1' ERR
exec > >(tee -i run_all.log) 2>&1
ulimit -c unlimited

echo "[RUN] Cleaning"
rm -rf obj_dir *.vcd *.fst

echo "[RUN] Verilating"
verilator -Wall -sv --cc --trace \
  -Mdir obj_dir --top-module f1_light \
  clktick.sv delay.sv lfsr_7.sv f1_fsm.sv f1_light.sv \
  --exe f1_light_tb_headless.cpp

echo "[RUN] Building"
make -j"$(nproc || sysctl -n hw.ncpu || echo 4)" -C obj_dir -f Vf1_light.mk Vf1_light

echo "[RUN] Running"
stdbuf -oL -eL ./obj_dir/Vf1_light || true
echo "[RUN] Exit code: $?"

echo "[RUN] Done. See run_all.log"
