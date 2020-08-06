#!/bin/bash

cd ..
rm -rf build
mkdir build
cd build
cmake ../
make -j 12
make spiffs_spiffs_bin
cd ../scripts
mkdir ESP32
cp ../build/ota_data_initial.bin ./ESP32/
cp ../build/partition_table/partition-table.bin ./ESP32/
cp ../build/bootloader/bootloader.bin ./ESP32/
cp ../build/homepoint.bin ./ESP32/
cp ../build/spiffs.bin ./ESP32
cd ESP32
python ../merge_bin_esp.py --output_name homepoint_espgeneric.bin --bin_path partition-table.bin ota_data_initial.bin bootloader.bin homepoint.bin spiffs.bin --bin_address 0x8000 0xd000 0x1000 0x10000 0x2b0000
cd ..
mkdir release
cp ./ESP32/output/homepoint_espgeneric.bin ./release

# Prepare M5Stack
cd ..
rm -rf build
mkdir build
cd build
cmake -DM5STACK=ON ../
make -j 12
make spiffs_spiffs_bin
cd ../scripts
mkdir M5Stack
cp ../build/ota_data_initial.bin ./M5Stack/
cp ../build/partition_table/partition-table.bin ./M5Stack/
cp ../build/bootloader/bootloader.bin ./M5Stack/
cp ../build/homepoint.bin ./M5Stack/
cp ../build/spiffs.bin ./M5Stack
cd M5Stack
python ../merge_bin_esp.py --output_name homepoint_m5stack.bin --bin_path partition-table.bin ota_data_initial.bin bootloader.bin homepoint.bin spiffs.bin --bin_address 0x8000 0xd000 0x1000 0x10000 0x2b0000
cd ..
mkdir release
cp ./M5Stack/output/homepoint_m5stack.bin ./release

cp readme.txt ./release
zip -r homepoint_release.zip ./release
