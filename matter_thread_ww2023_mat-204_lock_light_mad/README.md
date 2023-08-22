# Matter WorksWith 2023 MAT-204 Lock & Light MAD

## Overview

This tutorial provides an alternative to the video presentation for the WorksWith 2023, MAT-204 presentation: Smarter Home Designs with Matter and Thread.
We will go through the steps required to create a basic developement environment for creating an MAD (Matter accessory device), test it with a Matter ecosystem hub and modify it to add new functinality.
The guide below will provide information to:
- Create a project from a sample app (lock example) and test it with your development hardware.
- Create a Matter integration in the Google Developer Console.
- Update your project to match your integration and test the sample project with it.
- Modify the project to add new functionality (adding a light).
- Test again your project using both functionalities with your Matter ecosystem hub.

## Software Requirements

This example relies on the following software:
- Simplicity Studio 5; _version SV5.7.0.1_.
- Silicon Labs GSDK (Gecko Software Development Kit); _version 4.3.0_.
- Silicon Labs MGE (Matter GSDK Extension); _version 2.0.0-1.1_.

## Hardware Requirements

This example requires:
- A hardware development board:
  - This example uses the [BRD4186C](https://www.silabs.com/development-tools/wireless/xg24-rb4186c-efr32xg24-wireless-gecko-radio-board "BRD4186C") with a WSTK
- Google Nest Hub (2nd gen)
- Android smart phone running GHA (Google Home App)
  - Already connected to your Nest Hub

## Supplemental Requirements

This example requires:
- A wireless network with Internet access for your Matter ecosystem hub and Android device to use.
- An account for [Google's Developer Console](https://console.home.google.com/ "Google's Developer Console").
  - Make sure the account used for the _Developer Console_, your Matter ecosystem hub and Android device are the same or linked together properly to allow access.

## Guide

### Step 1 - Getting started

#### Creating the sample project of a lock over thread

Start by creating a sapmle project using the New Project Wizard.
![open the new project wizard](readme.md_assets/images/ww2023_mat-204_section_1_step_01.png)

Make sure to select the following information:
- Choose your target board; this example uses the _BRD4186C_.
- Select the desired SDK; this example uses _Gecko SDK Stuide v4.3.0_.
- Select the IDE / Toolchain; this example uses _Simplicity IDE / GNU ARM v10.3.1_.

![select the board brd4186c](readme.md_assets/images/ww2023_mat-204_section_1_step_02.png)
![select the gsdk 4.3.0](readme.md_assets/images/ww2023_mat-204_section_1_step_03.png)
![select the ide / toolchain gnu arm v10.3.1](readme.md_assets/images/ww2023_mat-204_section_1_step_04.png)

The final result should look something like this:
![select the board brd4186c, gsdk 4.3.0, ide/toolchain gnu arm v10.3.1](readme.md_assets/images/ww2023_mat-204_section_1_step_05.png)

Select the sample project using the filter keywords and checkboxes, choosing _Matter - SoC Lock over Thread_.
![select the soc lock over thread example](readme.md_assets/images/ww2023_mat-204_section_1_step_06.png)

Name the project and make sure _Link sdk and copy project sources_.
![select project name, link sdk and copu project sources and click finish](readme.md_assets/images/ww2023_mat-204_section_1_step_07.png)

#### Compile the project and upload to test board

From the contextual menu of the project, select _Build Project_.

![right-click project and select build project](readme.md_assets/images/ww2023_mat-204_section_1_step_08.png)

Open up the _Simplicity Commander_ tool from _Simplicity Studio_.

![click tools, select simplicity commander and click ok](readme.md_assets/images/ww2023_mat-204_section_1_step_09.png)

Choose the board you wish to work on; they are identified by a unique serial number.

![select the desired board serial number](readme.md_assets/images/ww2023_mat-204_section_4_step_03.png)

Go to the _Flash_ section and clear the flash of the test device to ensure removal of any previous testing, commissioning data, etc.

![go to the glash section](readme.md_assets/images/ww2023_mat-204_section_4_step_04.png)
![click on erase chip](readme.md_assets/images/ww2023_mat-204_section_4_step_05.png)

Upload a pre-compiled bootloader firmware binary and the firmware binary of the test project.
- A compatible bootloader firmware binary file is available with this example [here (s37)](/bootloader-storage-spiflash-single-1024k-BRD4186C-gsdk4.1.s37 "bootloader storage spiflash single 1024k BRD4186C gsdk4.1").
- You can find the binary of your example application the project folder under the _GNU ARM v10.3.1 - Default_ folder.

![click the browse button to select the bootloader or project firmware files and click flash to upload it](readme.md_assets/images/ww2023_mat-204_section_4_step_06.png)

### Step 2 - Testing with an ecosystem

#### Creating the project and Matter integration

Log into your _Google Developer Console_ and _Create a project_.

![click on create project](readme.md_assets/images/ww2023_mat-204_section_2_step_01.png)

![click on create project](readme.md_assets/images/ww2023_mat-204_section_2_step_02.png)

Name your project and click on _Create project_.

![name your project and click on create project](readme.md_assets/images/ww2023_mat-204_section_2_step_03.png)

From the side bar, click _Develop_.

![click on develop in the side bar](readme.md_assets/images/ww2023_mat-204_section_2_step_04.png)

Click on _Add integration_.

![click on add integration under develop](readme.md_assets/images/ww2023_mat-204_section_2_step_05.png)

Click on _Next: Develop_.

![click on next: develop at the bottom right](readme.md_assets/images/ww2023_mat-204_section_2_step_06.png)

Click _Next: Setup_.

![click on next: setup at the bottom right](readme.md_assets/images/ww2023_mat-204_section_2_step_07.png)

Enter a product name and select the device type _Lock_.

![enter a product name and select the device type lock](readme.md_assets/images/ww2023_mat-204_section_2_step_08.png)

Select the vendor ID to be a _Test VID_ and choose _0xFFF1_ from the dropdown.

![select test vid and 0xfff1 from the dropdown](readme.md_assets/images/ww2023_mat-204_section_2_step_09.png)

Click on _Save & continue_.

![click on save and continue at the bottom right](readme.md_assets/images/ww2023_mat-204_section_2_step_10.png)

You should now see your new integration listed under _Develop_.

![confirm new integration is visible under develop](readme.md_assets/images/ww2023_mat-204_section_2_step_11.png)

#### Modifying the example project to match the vendor & product information

From Simplicity Studio, open the project file by double-clicking the .slcp and browse to the _Zigbee Cluster Configurator_ under _Configuration Tools_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_02.png)

Select endpoint 0, choose _Enabled Clusters_ from the filter dropdown and scroll to _Basic Information_. Then, click the gear on the right to open the configuration page for this cluster.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_12.png)

Under the _Attributes_ tab, modify the following four attributes:
- VendorName; our example uses Silabs.
- VendorIDl; our example uses 0xFFF1.
- ProductName; SilabsTestLock.
- ProductID; 0x8006.

Note: make sure to use the same information you entered for the _Matter integration_ in the _Google Developer Console_ earlier.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_13.png)

Like in the previous step, compile the project and upload your new project firmware binary to your test device to update it with the newest changes. See the details [here](#compile-the-project-and-upload-to-test-board).

#### Testing with the ecosystem hub

From your Android device, open the GHA.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_14.png)

Select the devices icon.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_15.png)

Click on the _Add_ button.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_16.png)

Choose to setup a new Matter enabled device.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_17.png)

Choose a location for the new device.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_18.png)

Your Android device will now search for Matter devices.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_19.png)

Once a device is found, click the bottom right button to add it.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_20.png)

Click on the middle button to open the camera and scan the QR code on the LCD screen.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_21.png)

Click on the button to agree adding this device to your account.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_22.png)

The Android device will now search for the test MAD.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_23.png)

Eventually it will connect to the device.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_24.png)

Generate the Matter credentials.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_25.png)

Connect the test MAD to the network.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_26.png)

Check the test MAD network connectivity.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_27.png)

Connect the test MAD to the Google Home.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_28.png)

And finally display a message indicating the test MAD is now connected.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_29.png)

You will need to choose a location for your test MAD.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_30.png)

You can then change the name of the test MAD.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_31.png)

The Android device will indicate the test MAD is getting ready.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_32.png)

Finally, you'll be in your devices list and you should see your new test MAD, a lock device type.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_33.png)

Clicking the device's icon will toggle it's lock status. The image on the LCD and the LED next to it will change to match the new state.
You can also use the matching button on the WSTK to toggle the status of the LCD, LED and device icon on the Android device, too.

![](readme.md_assets/images/ww2023_mat-204_section_2_step_34.png)

![](readme.md_assets/images/ww2023_mat-204_section_2_step_35.png)

### Step 3 - Adding an extra endpoint

#### Adding a prerequesite software component

From Simplicity Studio, open the project file by double-clicking the .slcp and use the filters to find and install the _Level Control Server Cluster_ under _Software Components_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_01.png)

#### Creating the new endpoint

Then, open the _Zigbee Cluster Configurator_ under _Configuration Tools_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_02.png)

Click on _Add New Endpoint_ and select the _Matter Dimmable Light (0x0101) from the dropdown. Then, click _Create_ to add the new endpoint.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_03.png)

#### Configuring the new endpoint

Change the filter to _Enabled Clusters_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_04.png)

Open the configurations for the _Identify_ cluster.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_05.png)

Under _Attributes_ turn off the following four attributes:
- GeneratedCommandList
- AcceptedCommandList
- EventList
- AttributeList

![](readme.md_assets/images/ww2023_mat-204_section_3_step_06.png)

Under _Attribute Reporting_ change the _IdentifyTime_ and _ClusterRevision_ values for _Min Interval_ to 0.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_07.png)

Open the configurations for the _Groups_ cluster.

Under _Attributes_ turn off the following four attributes:
- GeneratedCommandList
- AcceptedCommandList
- EventList
- AttributeList

Under _Attribute Reporting_ change the _NameSupport and _CluisterRevision_ values for _Min Interval_ to 0.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_08.png)

Disable the _Scenes_ cluster by selected _Not Enabled_ in the dropdown on the down. It will be removed from the list.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_09.png)

Open the configurations for the _On/Off_ cluster.

Under _Attributes_ turn off the following four attributes:
- GeneratedCommandList
- AcceptedCommandList
- EventList
- AttributeList

Change the _OnOff_ and _StartUpOnOff_ values for _Storage Option_ to _NVM_ from the dropdowns. Also change the _StartUpOnOff_ value for _Default_ to 0xFF.
Note: You can ignore the out of range error, it is simply a UI glitch that will be corrected in a future update.

Finally, change the _FeatureMap_ value for _Default_ to 1.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_10.png)

Under _Attribute Reporting_, enable the _OnOff_ attribute.
Note: If any attribute is grayed out or otherwise you are unable to turn it on, simply go back to the _Attributes_ section and toggle the attribute off then on again.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_11.png)

Finally, change the _Min Interval_ value to 0 for the attributes shown below.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_12.png)

Open the configurations for the _Level Control_ cluster.

Under _Attributes_ turn on the following four attributes:
- _MinLevel_
- _MaxLevel_
- _CurrentFrequency_
- _MinFrequency_
- _MaxFrequency_
- _OnOffTransitionTime

Change the _CurrentLevel_ value for _Storage Option_ to _NVM_.

Change the _Default_ value of _CurrentLevel_ and _MinLevel_ to 0x01 and the _Default_ value of _OnLevel_ to 0xFF.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_13.png)

Under _Attributes_ turn on the following four attributes:
- _OnTransitionTime_
- _OffTransitionTime_
- _DefaultMoveRate_

Under _Attributes_ turn off the following four attributes:
- GeneratedCommandList
- AcceptedCommandList
- EventList
- AttributeList

Change the _StartUpCurrentLevel_ value for _Storage Option_ to _NVM_.

Change the _Default_ value for _DefaultMoveRate_ to 50, _StartUpCurrentLevel_ to 0xFF and _FeatureMap_ to 3.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_15.png)

Under _Attribute Reporting_ change the _Min Interval_ value for _CurrentLevel_, _RemainingTime_, _Options_, _StartUpCurrentLevel_ and _ClusterRevision_ to 0.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_16.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_17.png)

Open the configurations for the _Descriptor_ cluster.

Under _Attributes_ turn off the following four attributes:
- GeneratedCommandList
- AcceptedCommandList
- EventList
- AttributeList

Change the _ClusterRevision_ value for _Storage Option_ to _External_.

#### Modifying the application code to integrate the light endpoint

![](readme.md_assets/images/ww2023_mat-204_section_3_step_18.png)

Add the LightingManager.h and .cpp files to your project's _include_ and _src_ project respectively.
Note: You can get a copy of those files by creating a sample lighting project. You can also get a copy of the .h [here](matter_thread_ww2023_mat-204_lock_light_mad/include/LightingManager.h) and the .cpp [here](matter_thread_ww2023_mat-204_lock_light_mad/src/LightingManager.cpp).

![](readme.md_assets/images/ww2023_mat-204_section_3_step_19.png)

#### Modifying LightningManager.cpp

Modify the endpoint value from 1 to 2.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_20.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_21.png)

#### Modifying AppEvent.h

Add in the light event type and struct.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_22.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_23.png)

#### Modifying AppTask.h

Add an include for the _LightingManager.h_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_24.png)

Add missing function to manage the light endpoint.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_25.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_26.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_27.png)

#### Modifying AppTask.cpp

Add include for the _On/Off_ cluster server.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_28.png)

Add a define for the new LED. We'll be using LED 0, which is currently used as a status LED.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_29.png)

Add a LEDWidget to display the light endpoint status.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_30.png)

Add initialization code for the new callbacks.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_31.png)

Add the event handler for the light endpoint.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_32.png)

Modify the button event handler to add a case for toggling the light endpoint status (on/off).

![](readme.md_assets/images/ww2023_mat-204_section_3_step_33.png)

Add the code to initiate a light endpoint action.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_34.png)

Add the code to complete the light endpoint action.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_35.png)

Add code to handle events after the light action request.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_36.png)

Finally, add code to handle the light endpoint update to the on/off cluster.
Note: make sure to change the endpoint number from 1 to 2.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_37.png)

#### Modifying ZclCallbacks.cpp

Add an include for the _LightingManager.h_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_38.png)

Add the namespace.
Note: This may not be required, but was used by the lighting example and thus left it as-is.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_39.png)

Modify the Matter attribute callback to handle the ligh endpoint case.
We will add code for the dimming control but will not implement this feature in this tutorial.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_40.png)

Finally, add code for the On/Off cluster init callback.
Note: This functions is not used in this example but required to exist.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_41.png)

#### Modifying BaseApplication.cpp

Use the references search function to find the _BaseApplication.cpp_ file under _matter_2.0.0/examples/platform/silabs/efr32/BaseApplication.cpp_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_42.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_43.png)

Comment out the _LEDWidget_ for the status LED.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_44.png)

Comment out the initialization for the status LED _LEDWidget_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_45.png)

Finally, comment out all uses of that _LEDWidget_.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_46.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_47.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_48.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_49.png)

#### Modifying demo-ui.c

You can find _demo-ui.c_ under _matter_2.0.0/examples/platform/silabs/display/demo-ui.c_.

Add four new constants to define new images for the LCD to display the states of both endpoint (lock & light) simultaneously.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_50.png)

Modify the code to handle the display app function to allow for four states instead of only two.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_51.png)

Modify the call to add a second bollean operator to it (false on screen clear).

![](readme.md_assets/images/ww2023_mat-204_section_3_step_52.png)

#### Modifying demo-ui.h

You can find _demo-ui.h_ under _matter_2.0.0/examples/platform/silabs/display/demo-ui.h_.

Modify the signarue of function _demoUIDisplayApp_ to include two boolean arguments instead of one.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_53.png)

#### Modifying lcd.cpp

You can find lcd.cpp_ under _matter_2.0.0/examples/platform/silabs/display/lcd.cpp_.

Modify the initialization code to allow for two states to be set.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_54.png)

Modify the _WriteDemoUI_ function to have two boolean arguments and so it can handle the two states.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_55.png)

Finally, modify _WriteDemoUI_ to the call to _demoUIDisplayApp_ now passes both states instead of one.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_56.png)

#### Modifying lcd.h

You can find lcd.h_ under _matter_2.0.0/examples/platform/silabs/display/lcd.h_.

Modify the signature of function _WriteDemoUI_ to handle two boolean arguments instead of one.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_57.png)

Modify the code to handle two states instead of one.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_58.png)

#### Modifying AppTask.cpp (again)

Modify calls to _WriteDemoUI_ to handle the two boolean arguments.

![](readme.md_assets/images/ww2023_mat-204_section_3_step_59.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_60.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_61.png)

#### Modifying AppConfig.h

Add the images below to the header file.

```
#define LOCK_BULB_00                                                                                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00,     \
        0x80, 0x07, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xC0, 0x03,     \
        0x00, 0xF0, 0x0F, 0x00, 0xE0, 0x01, 0x80, 0x07, 0x00, 0xFE, 0x7F, 0x00, 0xE0, 0x01, 0x80, 0x07, 0x00, 0x1F, 0xF8, 0x00,     \
        0x00, 0x00, 0x80, 0x07, 0x80, 0x03, 0xC0, 0x01, 0x00, 0x00, 0x80, 0x07, 0xC0, 0x01, 0x00, 0x03, 0x00, 0x00, 0x80, 0x07,     \
        0xC0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07,     \
        0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18,     \
        0x00, 0x00, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0x07,     \
        0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x60, 0x00, 0x00, 0x06, 0xFE, 0xFF, 0xFF, 0x7F, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x01, 0x80, 0x01,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x03, 0xC0, 0x01, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFE, 0x7F, 0x00,     \
        0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF0, 0x0F, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xE0, 0x07, 0x00, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \

#define LOCK_BULB_10                                                                                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
        0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x00,     \
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00,     \
        0x80, 0x07, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xC0, 0x03,     \
        0x00, 0xF0, 0x0F, 0x00, 0xE0, 0x01, 0x80, 0x07, 0x00, 0xFE, 0x7F, 0x00, 0xE0, 0x01, 0x80, 0x07, 0x00, 0x1F, 0xF8, 0x00,     \
        0xE0, 0x01, 0x80, 0x07, 0x80, 0x03, 0xC0, 0x01, 0xE0, 0x01, 0x80, 0x07, 0xC0, 0x01, 0x00, 0x03, 0xE0, 0x01, 0x80, 0x07,     \
        0xC0, 0x00, 0x00, 0x03, 0xE0, 0x01, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07,     \
        0x18, 0x00, 0x00, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18,     \
        0xE0, 0x01, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x00, 0x00, 0x18, 0xE0, 0x01, 0x80, 0x07,     \
        0x18, 0x00, 0x00, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x60, 0x00, 0x00, 0x06, 0xFE, 0xFF, 0xFF, 0x7F, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x01, 0x80, 0x01,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x03, 0xC0, 0x01, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFE, 0x7F, 0x00,     \
        0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF0, 0x0F, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xE0, 0x07, 0x00, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \

#define LOCK_BULB_01                                                                                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00,         \
        0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04,     \
        0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00,     \
        0x40, 0x10, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x20, 0x04, 0x01,     \
        0x00, 0xF0, 0x0F, 0x00, 0x80, 0x20, 0x04, 0x01, 0x00, 0xF8, 0x1F, 0x00, 0x80, 0x20, 0x04, 0x01, 0x00, 0xFE, 0x7F, 0x00,     \
        0x00, 0x41, 0x82, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x04, 0x41, 0x82, 0x20, 0x80, 0x1F, 0xF8, 0x01, 0x04, 0x41, 0x82, 0x20,     \
        0x80, 0x07, 0xE0, 0x01, 0x08, 0x02, 0x40, 0x10, 0xC0, 0x03, 0xC0, 0x03, 0x08, 0x02, 0x40, 0x10, 0xC0, 0x03, 0xC0, 0x03,     \
        0x10, 0xF0, 0x0F, 0x08, 0xE0, 0x01, 0x80, 0x07, 0x10, 0xFE, 0x7F, 0x08, 0xE0, 0x01, 0x80, 0x07, 0x20, 0x1F, 0xF8, 0x04,     \
        0x00, 0x00, 0x80, 0x07, 0x80, 0x03, 0xC0, 0x01, 0x00, 0x00, 0x80, 0x07, 0xC0, 0x01, 0x80, 0x03, 0x00, 0x00, 0x80, 0x07,     \
        0xC0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07,     \
        0x18, 0x3C, 0x20, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x66, 0x24, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x42, 0x24, 0x18,     \
        0x00, 0x00, 0x80, 0x07, 0x18, 0x42, 0x28, 0x18, 0x00, 0x00, 0x80, 0x07, 0x18, 0x42, 0x28, 0x18, 0x00, 0x00, 0x80, 0x07,     \
        0x18, 0x42, 0x30, 0x18, 0x00, 0x00, 0x80, 0x07, 0x30, 0x66, 0x30, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x30, 0x3C, 0x20, 0x0C,     \
        0x00, 0x00, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x60, 0x00, 0x00, 0x06, 0xFE, 0xFF, 0xFF, 0x7F, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x01, 0x80, 0x01,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x03, 0xC0, 0x01, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFE, 0x7F, 0x00,     \
        0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF0, 0x0F, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xE0, 0x07, 0x00, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \

#define LOCK_BULB_11                                                                                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00,         \
        0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04,     \
        0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00,     \
        0x40, 0x10, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x20, 0x04, 0x01,     \
        0x00, 0xF0, 0x0F, 0x00, 0x80, 0x20, 0x04, 0x01, 0x00, 0xF8, 0x1F, 0x00, 0x80, 0x20, 0x04, 0x01, 0x00, 0xFE, 0x7F, 0x00,     \
        0x00, 0x41, 0x82, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x04, 0x41, 0x82, 0x20, 0x80, 0x1F, 0xF8, 0x01, 0x04, 0x41, 0x82, 0x20,     \
        0x80, 0x07, 0xE0, 0x01, 0x08, 0x02, 0x40, 0x10, 0xC0, 0x03, 0xC0, 0x03, 0x08, 0x02, 0x40, 0x10, 0xC0, 0x03, 0xC0, 0x03,     \
        0x10, 0xF0, 0x0F, 0x08, 0xE0, 0x01, 0x80, 0x07, 0x10, 0xFE, 0x7F, 0x08, 0xE0, 0x01, 0x80, 0x07, 0x20, 0x1F, 0xF8, 0x04,     \
        0xE0, 0x01, 0x80, 0x07, 0x80, 0x03, 0xC0, 0x01, 0xE0, 0x01, 0x80, 0x07, 0xC0, 0x01, 0x80, 0x03, 0xE0, 0x01, 0x80, 0x07,     \
        0xC0, 0x00, 0x00, 0x03, 0xE0, 0x01, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C,     \
        0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07,     \
        0x18, 0x3C, 0x20, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x66, 0x24, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x42, 0x24, 0x18,     \
        0xE0, 0x01, 0x80, 0x07, 0x18, 0x42, 0x28, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x18, 0x42, 0x28, 0x18, 0xE0, 0x01, 0x80, 0x07,     \
        0x18, 0x42, 0x30, 0x18, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x66, 0x30, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x30, 0x3C, 0x20, 0x0C,     \
        0xE0, 0x01, 0x80, 0x07, 0x30, 0x00, 0x00, 0x0C, 0xE0, 0x01, 0x80, 0x07, 0x60, 0x00, 0x00, 0x06, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x60, 0x00, 0x00, 0x06, 0xFE, 0xFF, 0xFF, 0x7F, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x01, 0x80, 0x01,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x80, 0x03, 0xC0, 0x01, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x03, 0xC0, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x06, 0x60, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFE, 0x7F, 0x00,     \
        0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x0F, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x1F, 0xF8, 0x7F,     \
        0x00, 0xFC, 0x3F, 0x00, 0xFE, 0x3F, 0xFC, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0x3F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F,     \
        0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0x1F, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xF0, 0x0F, 0x00,     \
        0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x7F, 0x00, 0xE0, 0x07, 0x00, 0xFC, 0xFF, 0xFF, 0x3F,     \
        0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
```

![](readme.md_assets/images/ww2023_mat-204_section_3_step_62.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_63.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_64.png)

![](readme.md_assets/images/ww2023_mat-204_section_3_step_65.png)


### Step 4 - Testing again with our new features

#### Compile the project and upload

![](readme.md_assets/images/ww2023_mat-204_section_1_step_09.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_03.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_04.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_05.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_06.png)

Once the upload is complete, add your test MAD to your hub as you did previously [here](#Testing-with-the-ecosystem-hub).

#### Testing the new features

You'll notice a few differences with your modified test MAD.

First, it shows up as two icons in _GHA_, one light and one lock.
Pressing on the lock will icon toggle the lock state as it did previously, updating the LCD and associated LED to show the new status.
Pressing on the light icon will also toggle the status of the LCD to display the new light endpoint status and toggle the associated LED.

![](readme.md_assets/images/ww2023_mat-204_section_4_step_07.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_08.png)

![](readme.md_assets/images/ww2023_mat-204_section_4_step_09.png)

#### {END - Matter WorksWith 2023 MAT-204 Lock & Light MAD}