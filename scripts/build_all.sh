#!/bin/bash

cd ..
# Prepare M5Stack
rm -rf build
cp scripts/TFT_eSPI/User_Setup_M5Stack.h main/libraries/TFT_eSPI/User_Setup.h
mkdir build
cd build
cmake -DM5STACK=ON ../
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

cd ..
cp scripts/TFT_eSPI//User_Setup_espgeneric.h main/libraries/TFT_eSPI/User_Setup.h
rm -rf build
mkdir build
cd build
cmake ../
make -j 12
make spiffs_spiffs_bin
cd ../scripts
mkdir ESP32
cp ../build/partition_table/partition-table.bin ./ESP32/
cp ../build/bootloader/bootloader.bin ./ESP32/
cp ../build/homepoint.bin ./ESP32/
cp ../build/spiffs.bin ./ESP32
cd ESP32
python ../merge_bin_esp.py --output_name homepoint_espgeneric.bin --bin_path partition-table.bin bootloader.bin homepoint.bin spiffs.bin --bin_address 0x8000 0x1000 0x10000 0x310000
cd ..
mkdir release
cp ./ESP32/output/homepoint_espgeneric.bin ./release
cp readme.txt ./release
zip -r homepoint_release.zip ./release
