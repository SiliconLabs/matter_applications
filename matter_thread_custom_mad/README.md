<table border="0">
  <tr>
    <td align="left" valign="middle">
    <h1>Tutorial: Matter over OpenThread Custom Matter Device</h1>
  </td>
  <td align="left" valign="middle">
    <a href="https://www.silabs.com/wireless/matter">
      <img src="http://pages.silabs.com/rs/634-SLU-379/images/WGX-transparent.png"  title="Silicon Labs Gecko and Wireless Gecko MCUs" alt="EFM32 32-bit Microcontrollers" width="100
      "/>
    </a>
     <a href="https://www.silabs.com/wireless/matter">
      <img src="images/set-up-software.png
"  title="Silicon Labs Gecko and Wireless Gecko MCUs" alt="EFM32 32-bit Microcontrollers" width="100
"/>
  </td>
  </tr>
</table>

# Color Control Light MAD #
![Type badge](https://img.shields.io/badge/Type-Virtual%20application-green)
![Technology badge](https://img.shields.io/badge/Technology-Matter-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v0.4.0-green)

## Summary ##

This tutorial will show you how to create and build a dimmable RGB light app (Color Control Light Matter Accessory Device) from the Silicon Labs Matter Github (SMG) repository's existing template app for ThunderBoard Sense 2 (BRD4166A).

## Gecko SDK version ##

SMG 0.4.0

## Hardware Required ##

- Raspberry Pi 4
- 2 x Silabs Thunderboard Sense 2 (TBS2 -- BRD4166A) 
- Ubuntu Linux build environment on hardware or VirtualBox already used to build an initial MAD Light example from SMG

 
## Connections Required ##
 
- Ethernet or Wi-Fi for RasPi, Linux build machine
- Micro USB for TBS2/RasPi
- Micro USB for TBS2/Linux 
 
## Setup ##
 
### Prerequisites ###

This tutorial expects you to have built a MAD on your local setup, following either:
- The [Lab Manual and updates](https://community.silabs.com/s/article/MAT-301-Lab-Update-Works-With-2022?language=en_US) from the Silicon Labs WorksWith2022 MAT-301 seminar

or:

- The guides found in SMG:
    - [Matter over Thead demo overview](https://github.com/SiliconLabs/matter/blob/release_0.3.0/docs/silabs/thread/DEMO_OVERVIEW.md)
    - [Matter lighting demo](https://github.com/SiliconLabs/matter/tree/release_0.4.0/examples/lighting-app/efr32)
    - [Creating a custom application](https://github.com/SiliconLabs/matter/blob/release_0.4.0/silabs_examples/template/efr32/HOW_TO_CREATE_A_CUSTOM_APP.md)

_Note: Unless otherwise specified, the commands in the tutorial are run from inside the directory of the cloned Silabs Matter repository eg._ **~/matter$**.

## How It Works ##
 
### Step 0: Prerequisites ###
Before beginning your Silicon Labs Matter over Thread project be sure you have satisfied all of the Matter Hardware and Matter Software Requirements.

_Note: While the build in the MAD Light tutorial was done entirely inthe

### Step 1: Set up your new project structure ###
-Make a copy of the folder silabs_examples/template in same directory and name the folder **colorLight**
- Rename the files inside as follow:
template_DataModel_config → colorLight_DataModel_config
sl_template.matter → colorLight.matter
sl_template.zap →  colorLight.zap

At this point, the folder structure should look like this:
 
## .sls Projects Used ##
 
N/A; projects are managed in github



