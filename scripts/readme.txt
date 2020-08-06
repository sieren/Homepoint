Steps to install:

1. Download ESPTools from https://github.com/espressif/esptool/releases
2. Install ESPTools with python setup.py install
3. Run esptool.py write_flash -fs 4MB -fm dout 0x0 homepoint_m5stack_full.bin (or homepoint_espgeneric_full.bin)

-----

The generic NodeMCU ESP32 is built with the following pins:

+-----------+-----+
|   TYPE    | PIN |
+-----------+-----+
| TFT_CS    |   5 |
| TFT_DC    |   4 |
| TFT_MOSI  |  23 |
| TFT_SCLK  |  18 |
| TFT_RST   |  22 |
| TFT_MISO  |  19 |
| TFT_LED   |  15 |
| TOUCH_CS  |  14 |
| TOUCH_IRQ |   2 |
+-----------+-----+

To translate the Pins to other boards, please refer to the following diagram:
https://cdn.shopify.com/s/files/1/1509/1638/files/ESP_-_32_NodeMCU_Developmentboard_Pinout_Diagram.jpg
