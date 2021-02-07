# M5Core2 Library

English | [中文](README_cn.md)

<img src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_01.webp" alt="M5Core2_P1" width="350" height="350">


<img src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_02.webp" alt="M5Core2_P2" width="350" height="350">

## Description

**M5Core2** is the second generation core device in the M5Stack development kit series, which further enhances the functions of the original generation of cores. 

The MCU is an ESP32 model D0WDQ6-V3 and has dual core Xtensa® 32-bit 240Mhz LX6 processors that can be controlled separately. WiFi and Bluetooth are supported as standard and it includes an on board 16MB Flash and 8MB PSRAM, USB TYPE-C interface for charging, downloading of programs and serial communication, a 2.0-inch integrated capacitive touch screen, and a built-in vibration motor. 

M5Core2 also features a built-in RTC module which can provide accurate timing. The power supply is managed by an AXP192 power management chip, which can effectively control the power consumption of the base and a built-in green LED power indicator helps to notify the user of battery level. The battery capacity has been upgraded to 390mAh, which can power the core for much longer than the previous model. 

The M5Core2 retains the SD card slot and speakers. However, in order to ensure higher quality sound output, the I2S digital audio interface power amplifier chip is used to effectively prevent signal distortion. There are independent power and reset buttons on the left side and bottom of the base. 

The 3 icons on the front of the screen are capacitive buttons which are programmable. There is a small expansion board on the back of the base with a 6-axis IMU sensor and microphone.

## PinMap

**LCD & TF card**

LCD ：320x240
TF card Maximum size 16GB

<table>
 <tr><td>ESP32 Chip</td><td>GPIO38</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td><td>GPIO15</td><td></td><td> </td><td> </td></tr>
 <tr><td>AXP192 Chip</td><td> </td><td> </td><td> </td><td> </td><td> </td><td>AXP_IO4</td><td>AXP_DC3</td><td>AXP_LDO2</td></tr>
 <tr><td>ILI9342C</td><td>MISO</td><td>MOSI</td><td>SCK</td><td>CS</td><td>DC</td><td>RST</td><td>BL</td><td>PWR</td></tr>
</table>

<table>
<tr><td>ESP32 Chip</td><td>GPIO38</td><td>GPIO23</td><td>GPIO18</td><td>GPIO4</td></tr>
<tr><td>TF Card</td><td>MISO</td><td>MOSI</td><td>SCK</td><td>CS</td></tr>
</table>

**CAP.TOUCH**

<table>
 <tr><td>ESP32 chip</td><td>GPIO21</td><td>GPIO22</td><td>GPIO39</td></tr>
 <tr><td>AXP192</td><td></td><td></td><td></td><td>AXP_IO4</td></tr>
 <tr><td>FT6336U</td><td>SDA</td><td>SCL</td><td>INT</td><td>RST</td></tr>
</table>

**Mic & NS4168**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO12</td><td>GPIO0</td><td>GPIO2</td><td>AXP_IO2</td><td>GPIO34</td></tr>
 <tr><td>NS4168</td><td>BCLK</td><td>LRCK</td><td>DATA</td><td>SPK_EN</td> </td></tr>
 <tr><td>Mic</td><td></td><td>CLK</td><td></td><td></td><td>DATA</td></tr>
</table>

**AXP Power Indicator Light & Vibration motor**

<table>
 <tr><td>AXP192</td><td>AXP_IO1</td><td>AXP_LDO3</td></tr>
 <tr><td>Green LED</td><td>Vcc</td><td></td></tr>
 <tr><td>Vibration motor</td><td></td><td>Vcc</td></tr>
</table>

**RTC**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO21</td><td>GPIO22</td><td></td></tr>
 <tr><td>AXP192</td><td></td><td></td><td>AXP_PWR</td></tr>
 <tr><td>BM8563</td><td>SDA</td><td>SCL</td><td>INT</td></tr>
</table>

**IMU(3-axis gyroscope & 3-axis accelerometer)**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO21</td><td>GPIO22</td></tr>
 <tr><td>MPU6886</td><td>SDA</td><td>SCL</td></tr>
</table>

**USB to serial chip**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO1</td><td>GPIO3</td></tr>
 <tr><td>CP2104</td><td>RXD</td><td>TXD</td></tr>
</table>


**Internal I2C connection**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO21</td><td>GPIO22</td></tr>
 <tr><td>MPU6886</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>AXP192</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>BM8563</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>FT6336U</td><td>SDA</td><td>SCL</td></tr>
</table>


## M5Core2 M-BUS Schematic diagram

<img class="pic" src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_mbus.webp" width = "50%">

## M5PORT EXPLAIN

<table>
      <thead>
         <th>PORT</th>
         <th>PIN</th>
         <th>Note:</th>
      </thead>
      <tbody>
      <tr>
         <td>PORT-A(Red)</td>
         <td>G32/33</td>
         <td>I2C</td>
      </tr>
    </tbody>
</table>

## More Information

[Product docs](https://docs.m5stack.com/#/en/core/core2)