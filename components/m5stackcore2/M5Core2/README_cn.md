# M5Core2 Library

中文 | [English](README_cn.md)

<img src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_01.webp" alt="M5Core2_P1" width="350" height="350">


<img src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_02.webp" alt="M5Core2_P2" width="350" height="350">


## 描述

**M5Core2** 是M5Stack开发套件系列中第二代主机，在原有一代主机基础上对功能进一步加强，硬件功能更加齐全。主控ESP32型号为D0WDQ6-V3，具有两个可以单独控制的 Xtensa® 32-bit LX6 处理器，主频高达240Mhz，支持WiFi与蓝牙功能，板载16MB Flash与8MB PSRAM，可通过TYPE-C接口下载程序，强劲的配置满足复杂应用的资源开销。正面搭载一块2.0寸一体化电容式触摸屏，为用户带来更流畅的人机交互体验。机身内置震动马达，可提供触觉回馈和震动提醒功能。内建的RTC模块可提供精准计时功能。电源部分搭载AXP192电源管理芯片可有效控制机身功耗，内置绿色电源指示灯，配备390mAh电池，续航时间更持久。同时机身内配备了SD卡槽与扬声器，为了保证获得更高质量的声音效果，采用I2S数字音频接口的功放芯片，能有效防止信号失真。在机身的左侧和底部配有独立的电源按键与重启(RST)按键，屏幕正面的3个圆点属于触摸屏的一部分，可通过编写程序设置热区映射为3个虚拟按键。机身背部有一块扩展小板，板上带有6轴IMU传感器与麦克风。

## 管脚映射

**LCD 屏幕 & TF Card**

LCD 像素：320x240
TF 卡最大支持 16GB

<table>
 <tr><td>ESP32 Chip</td><td>GPIO38</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td><td>GPIO15</td><td></td><td> </td><td> </td></tr>
 <tr><td>AXP192 Chip</td><td> </td><td> </td><td> </td><td> </td><td> </td><td>AXP_IO4</td><td>AXP_DC3</td><td>AXP_LDO2</td></tr>
 <tr><td>ILI9342C</td><td>MISO</td><td>MOSI</td><td>SCK</td><td>CS</td><td>DC</td><td>RST</td><td>BL</td><td>PWR</td></tr>
</table>

<table>
<tr><td>ESP32 Chip</td><td>GPIO38</td><td>GPIO23</td><td>GPIO18</td><td>GPIO4</td></tr>
<tr><td>TF Card</td><td>MISO</td><td>MOSI</td><td>SCK</td><td>CS</td></tr>
</table>

**CAP.TOUCH触摸屏**

<table>
 <tr><td>ESP32 chip</td><td>GPIO21</td><td>GPIO22</td><td>GPIO39</td></tr>
 <tr><td>AXP192</td><td></td><td></td><td></td><td>AXP_IO4</td></tr>
 <tr><td>FT6336U</td><td>SDA</td><td>SCL</td><td>INT</td><td>RST</td></tr>
</table>

**麦克风 & NS4168功放**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO12</td><td>GPIO0</td><td>GPIO2</td><td>AXP_IO2</td><td>GPIO34</td></tr>
 <tr><td>NS4168</td><td>BCLK</td><td>LRCK</td><td>DATA</td><td>SPK_EN</td> </td></tr>
 <tr><td>Mic</td><td></td><td>CLK</td><td></td><td></td><td>DATA</td></tr>
</table>

**AXP电源指示灯 & 震动马达**

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

**IMU(3轴陀螺仪+3轴加速计)**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO21</td><td>GPIO22</td></tr>
 <tr><td>MPU6886</td><td>SDA</td><td>SCL</td></tr>
</table>

**USB转串口下载**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO1</td><td>GPIO3</td></tr>
 <tr><td>CP2104</td><td>RXD</td><td>TXD</td></tr>
</table>


**内部I2C连接**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO21</td><td>GPIO22</td></tr>
 <tr><td>MPU6886</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>AXP192</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>BM8563</td><td>SDA</td><td>SCL</td></tr>
 <tr><td>FT6336U</td><td>SDA</td><td>SCL</td></tr>
</table>

## M5Core2 M-BUS示意图

<img class="pic" src="https://docs.m5stack.com/assets/img/product_pics/core/core2/core2_mbus.webp" width = "50%">

## M5Core2 端口说明

<table>
      <thead>
         <th>PORT</th>
         <th>PIN</th>
         <th>备注:</th>
      </thead>
      <tbody>
      <tr>
         <td>PORT-A(红色)</td>
         <td>G32/33</td>
         <td>I2C</td>
      </tr>
    </tbody>
</table>

## 更多信息

[产品文档](https://docs.m5stack.com/#/zh_CN/core/core2)
