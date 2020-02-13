#!/bin/bash

cd ..
cd build
make -DM5STACK=ON ../
make -j 12
make spiffs_spiffs_bin
cd ../scripts
mkdir M5Stack
cp ../build/partition_table/partition-table.bin ./M5Stack/
cp ../build/bootloader/bootloader.bin ./M5Stack/
cp ../build/homepoint.bin ./M5Stack/
cp ../build/spiffs.bin ./M5Stack
cd M5Stack
python ../merge_bin_esp.py --output_name homepoint_m5stack.bin --bin_path partition-table.bin bootloader.bin homepoint.bin spiffs.bin --bin_address 0x8000 0x1000 0x10000 0x310000
cd ..
mkdir release
cp ./M5Stack/output/homepoint_m5stack.bin ./release
