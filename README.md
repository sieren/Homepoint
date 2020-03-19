<p align="center">
  <img src="https://raw.githubusercontent.com/sieren/Homepoint/master/media/logo.png" width="320" alt="homepoint logo" />
</p>
<p align="center">
<img alt="issues" src="https://travis-ci.org/sieren/Homepoint.svg?branch=master">
<a href="https://github.com/sieren/Homepoint/issues"><img src="https://img.shields.io/github/issues/sieren/Homepoint.svg"/></a>
<a href="#donations"><img  src="https://yourdonation.rocks/images/badge.svg" alt="Donations Badge"></a>
</p>

### :house: Control your MQTT/HomeKit Smart Home Devices from an ESP32-powered screen :house: 

<p align="center">
  <img src="https://raw.githubusercontent.com/sieren/Homepoint/master/media/img1.jpg" width="400"><img src="https://raw.githubusercontent.com/sieren/Homepoint/master/media/img2.gif" width="400"> 
</p>

Homepoint is a screen-based interface for MQTT & HomeKit-connected Smarthome devices that runs on the cheaply available ESP32 Chipset.  

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Build](#build)
- [Configuration](#configuration)
  - [Scenes & Devices](#devices)
  - [WiFi & MQTT](#credentials)
  - [NTP Time (optional)](#ntp)
  - [Examples](#examples)
- [Roadmap](#roadmap)
- [Known bugs & issues](#known-bugs)
- [Contributing](#contributing)
- [Buy me a coffee](#donations)
- [License](#license)

<a name="features"></a>

# :heavy_check_mark: Features

- **See and trigger scenes** (groups of devices) on the front screen.

- **Trigger individual devices** by diving into scenes (by tapping the indicator or long pressing a button).

+ **See partially switched on scenes** with multiple devices at a glance.

+ Support for **temperature, humidity, air quality sensors & Tasmota energy readings**.

+ **Remote Configuration**: Configure and reboot HomePoint through a web interface. No need to reflash.

+ **HomeKit Switch Support** (Setup Automations in Home App to toggle from Homepoint)*

+ Supports both **Touch Screen or Button based navigation**.

+ **Reactive**: Screen updates whenever devices are triggered from elsewhere.

+ **Easy configuration** through a JSON file, no additional code required.

+ **Screensaver** saves power by switching off screen after 10 minutes.

+ **Statusbar** shows connectivity for WiFi and MQTT as well as time.

+ **Failsafe Mode**: In case of corrupt configuration, failsafe mode for remote access is provided.

+ **Pre-compiled binaries**: Just flash HomePoint on your ESP32 device without compiling.

*currently on experimental branch.
<a name="demo">
  
# :camera: Demo

[![HomePoint Youtube demo](https://raw.githubusercontent.com/sieren/Homepoint/master/media/youtube.png)](https://www.youtube.com/watch?v=bqzpkvtQSvY "HomePoint Youtube demo")

<a name="installation"></a>

# :house: Installation

HomePoint comes in 2 pre-compiled .bin files ready to be flashed on either a generic ESP32 Module or M5Stack.
The PIN Layout required for generic ESP32 modules can be found inside the readme.txt in the [releases](https://github.com/sieren/Homepoint/releases).

Steps to install:

1. Download ESPTools from [https://github.com/espressif/esptool/releases](https://github.com/espressif/esptool/releases)
2. Install ESPTools with `python setup.py install`
3. Run `esptool.py write_flash -fs 4MB -fm dout 0x0 homepoint_espgeneric.bin` (or homepoint_m5stack.bin for M5Stack)

Alternatively, watch the installation video below:

[![HomePoint Installation demo](https://raw.githubusercontent.com/sieren/Homepoint/master/media/installvideo.png)](https://www.youtube.com/watch?v=Ql9BQKl_Yuk "HomePoint Install demo")

<a name="build"></a>

# :house: Build

Homepoint was created using the [ESP-IDF SDK](https://github.com/espressif/esp-idf).

In order to build HomePoint you need ESP-IDF from the `release/v4.0` branch from the ESP-IDF git repository.  

| Software       | Version                                                                                                                 |
| :------------- | :----------------------------------------------------------------------------------------------------------------------:|
|  ESP-IDF       | [release/v4.0](https://github.com/espressif/esp-idf/tree/release/v4.0))                                                           |
|  Toolchain     | [5.2.0](https://docs.espressif.com/projects/esp-idf/en/stable/get-started-cmake/index.html#step-1-set-up-the-toolchain) |

**Setup your build environment** if necessary:

Please refer to the [documentation](https://docs.espressif.com/projects/esp-idf/en/v4.0/), especially the [prerequisites](https://docs.espressif.com/projects/esp-idf/en/v4.0/get-started/linux-setup.html) but in general the steps are as follows:

On Linux:

```
$ mkdir ~/esp
$ cd ~/esp
$ git clone -b v4.0 --recursive https://github.com/espressif/esp-idf.git
$ cd esp-idf
$ ./install.sh
$ . ~/esp/esp-idf/export.sh
$ cd ..
```

**Clone and build** HomePoint with the following commands:

```bash
$ git clone https://www.github.com/sieren/homepoint.git
$ cd homepoint
$ git submodule update --init --recursive
$ mkdir build && cd build
$ cmake ../
$ make spiffs_spiffs_bin
$ make -j 8 flash
```  

For use with a Button-Based interface:

```
$ cmake -DBUTTONS=ON ../
```

For out-of-the-box use with M5STACK:

```
$ cmake -DM5STACK=ON ../
```

Depending on your hardware, you may have to uncommented the relevant sections in `main/libraries/TFT_ESPI/User_Setup.h`.
  
Other Homepoint configuration settings like the Screensaver can be configured in `main/config/Config.h`.

<a name="configuration"></a>

# :twisted_rightwards_arrows: Configuration

The configuration lives in `data/config.json`.

<a name="devices"></a>
## Devices & Scenes

HomePoint supports two types of Scenes with a selection of devices each:

| SCENE Types   | DEVICE Types                                                                             |
| --------------| ----------------------------------------------------------------------------------------- |
| Light, Switch | (none, all devices expected to be switches)                                               |
| Sensor        | display one or two values at once with customizable icons                                 |

**Grouped Sensors** support up to two devices (due to screen space).  
**Lights & Switches** support an infinite number of devices in a group.

Some Sensors use **JSON** as a data-format, in which case a **key** can be defined. These can be on any level of the sensor json payload, for Tasmota smart plugs the key value "Power" can be used to display the current power consumption.

<a name="credentials"></a>
## WiFi & MQTT Credentials

WiFi and MQTT credentials are to be defined on the top level.

<a name="ntp"></a>
## NTP Time (optional)

In order to set the correct timezone, copy & paste your [NTP TZ Setting](https://remotemonitoringsystems.ca/time-zone-abbreviations.php) to the `timezone` key.


<a name="examples"></a>

## Example

```json
{
  "wifi": "MyWifiSSID",
  "password": "My Wifi Password",
  "login": "admin",
  "webpass": "admin",
  "mqttbroker": "mqtt://192.168.1.2",
  "mqttusername": "mqttusername",
  "mqttpasswd": "mymqttpassword",
  "timezone": "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
  "scenes": [{
    "name": "Living Room",
    "type": "Light",
    "icon": "livingroom",
    "devices": [{
      "name": "Side Light",
      "setTopic": "lights/hue/00:17:88:01:02:3c:2a:6d-0b/set/on",
      "getTopic": "hue/status/lights/Hue color lamp 1",
      "onValue": "true",
      "offValue": "false"
    }]
  },
  {
    "name": "Bedroom",
    "type": "Sensor",
    "icon": "door",
    "devices": [{
      "name": "Temperature DHT Sensor",
      "type": "oneValue",
      "jsondata": true,
      "firstIcon": "temperature_small",
      "firstKey": "temperature",
      "getTopic": "bedroom/esptemp"
    }]
  }]
}
```

A more **fully-configured example** is available [here](https://github.com/sieren/Homepoint/blob/master/data/config.json).


<a name="roadmap"></a>

# :crystal_ball: Roadmap

- [x] Release 0.01
- [x] Add Button support
- [x] Build on CI
- [x] Migrate to 4.0 Release Branch
- [ ] Productionize [HomeKit Support](https://github.com/sieren/Homepoint/pull/10)
- [ ] Better NTP Time Syncing
- [ ] Add Unit-Tests

<a name="known-bugs"></a>

# :bug: Known bugs & issues :bug:

- ESP IDF 4.0 has not been released yet and is in influx. Therefore breaking changes might happen.

> If you find any bugs or need assistance, don't hesitate and open an issue :)

<a name="contributing"></a>

# :revolving_hearts: Contributing

Contributions are welcome! Just hack away and open a Pull Request.
But please follow the style of the code that's already there (ie. no tabs, spacing).

<a name="donations"></a>

# :coffee: Buy me a coffee

I have developed HomePoint in my free time because I wanted to learn more about C++ on ESP32
and to solve a concrete problem in my smarthome.  
As development kept on going, I thought about ways to make this project open source and more accessible and user-friendly for the maker-community.  

If you enjoy using HomePoint or this work, consider a small donation. But don't worry, I'll continue working on this anyway ;-). 

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=WC74EF774344J&lc=US&item_name=homepoint&no_note=0&no_shipping=1&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHostedJ)

### Thanks!!

<a name="license"></a>

# :scroll: License

MIT © [Matthias Frick](https://github.com/sieren) 

