#!/bin/bash

for file in `find -type f | ls "../../../riscv-cpu/verilator/build/"*.bin`
do
  echo "$file"
  make IMAGE=$file emu
#  ./obj_dir/VsimTop $file 2>/dev/null
done
