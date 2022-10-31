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
***Note:* In the WorksWith2022 seminar, you may have used the Wirebridge USB Flashstick; the provided RCP is not compatible with this project.**

 
## Connections Required ##
 
- Ethernet or Wi-Fi for RasPi, Linux build machine
- Micro USB for TBS2/RasPi
- Micro USB for TBS2/Linux 
 

## Overview ##

You have already built and interacted with the simple MAD Lighting App on a Thread network running on the OTBR on RaspberryPi. This tutorial is the next step; using a project template and modifying it to add features. 

We will be creating a project from the template, using the ZAP tool to configure the endpoints, clusters and attributes, and then adding code to make it all work.  

To simplify the experience and provide a resource to check against, we will be building a project substantially similar to one of several examples Silicon Labs provides within SMG. This tutorial is not focused on writing code, so we'll be copying the files and code we need from the **sl-newLight** example, but we will still need to make adjustments based on this particular project.

## Prerequisites ##
<!--- 
Silabs Apps requirement seems to need Prerequisites to not be called a Step 0 (?) and maybe come before Setup. Alex to check
--->
This tutorial expects you to have built a MAD on your local setup, following either:
- The [Lab Manual and updates](https://community.silabs.com/s/article/MAT-301-Lab-Update-Works-With-2022?language=en_US) from the Silicon Labs WorksWith2022 MAT-301 seminar

or:

- The guides found in SMG:
    - [Matter over Thead demo overview](https://github.com/SiliconLabs/matter/blob/release_0.3.0/docs/silabs/thread/DEMO_OVERVIEW.md)
    - [Matter lighting demo](https://github.com/SiliconLabs/matter/tree/release_0.4.0/examples/lighting-app/efr32)
    - [Creating a custom application](https://github.com/SiliconLabs/matter/blob/release_0.4.0/silabs_examples/template/efr32/HOW_TO_CREATE_A_CUSTOM_APP.md)
- The **RaspberryPi OTBR .img** and **RCP** ***must_ be from SMG 0.4.0*** for this tutorial

***Note:** While the build in the MAD Light tutorial was done entirely in the Linux console, this tutorial works from the Ubuntu GUI. However, the only component that **requires** the Linux or Mac GUI is the **ZAP tool**. If necessary, this project can be built in the console without invoking the ZAP tool but it is best to use the tool if possible.*


***Note:** Unless otherwise specified, the commands in the tutorial are run from inside the directory of the cloned Silabs Matter repository, eg. **~/matter$**.*


- Silicon Labs Matter repository
You should already have the SiliconLabs Matter repository with submodules, but you may be on a prior version. If so, check out the current version tag seen in [SMG]():

   `$ git checkout release_0.4.0`  
   You may need to stash any changes you've made in the prior version:
   `$ git stash`
    Clone the SiliconLabs Matter repository with its submodules:
   `
   $ git clone --recurse-submodules https://github.com/SiliconLabs/matter.git
   `


## Step 1: Set up your new project structure ##

Make a copy of the folder **silabs\_examples/template** in same directory and name the folder **colorLight**

Rename the files inside as follows:

*   ***template*\_DataModel\_config → *colorLight*\_DataModel\_config**
*   **sl\_*template*.matter → *colorLight*.matter**
*   **sl\_*template*.zap →  *colorLight*.zap**

At this point, the folder structure should look like this:

![Project folder structure](images/image2022-10-19_17-18-13.png)

## Step 2: Modify the GN build file ##


Modify the file **BUILD&period;gn** inside **silabs\_examples/colorLight/colorLight\_DataModel\_config** to properly refer to your newly created project:  

```cpp
import("//build_overrides/chip.gni")
import("${chip_root}/examples/common/pigweed/pigweed_rpcs.gni")
import("${chip_root}/src/app/chip_data_model.gni")

config("config") {
  include_dirs = [ "include" ]
}

chip_data_model("colorLight_DataModel_config") {
  zap_file = "colorLight.zap"

  zap_pregenerated_dir = "${chip_root}/zzz_generated/colorLight/zap-generated"
  is_server = true
}
```

## Step 3: Use ZAP tool ##

Run the ZAP tool, modify the clusters to fit the application requirement and save it. 

*   Run the **ZAP tool** with the .zap file you just created at Step 1 as the argument:

    `$ ./scripts/tools/zap/run_zaptool.sh silabs_examples/colorLight/colorLight_DataModel_config/colorLight.zap`

    ***Note:* If you do not have access to a GUI version of Ubuntu or MacOS to run ZAP, this command will fail.** For now you can follow along here to understand the process and then copy the *zap-generated* directory from */zzz_generated/sl-newLight/* to *zzz_generated/colorLight*, and continue to modify and build the project as described.** 



On startup, the GUI should look like this:

 ![ZAP GUI](images/image2022-10-18_15-17-1.png)

*   Select **Endpoint 1** from the endpoint list on the left panel, this endpoint represents our RGB light functionality. 

A menu of its clusters, listed in groups will show up:

![Cluster menu](images/image2022-10-18_15-20-47.png) 
*   Navigate to the General group and make sure that **On/Off** cluster and **Level Control** cluster are both enabled with Server type

These two clusters are responsible for turning on/off the light and controlling the brightness, respectively.

![Lighting clusters](images/image2022-10-18_15-28-15.png)
*   Select the configuration option of the **Level Control** cluster:
![Level Control configure cog](images/image2022-10-28_16-27-53.png)
*   Set the default value of **CurrentLevel** to 0xFE instead of 0:
![CurrentLevel setting](images/image2022-10-28_16-33-2.png)
*   Navigate to the lighting group and make sure that the **Color Control** cluster is enabled with Server type.
![Color Control cluster](images/image2022-10-18_15-41-22.png)
    * This cluster provides the interface to for the remote client to control the color of the RGB light.  
    




*   Select the configure option of the **Color Control** cluster:
![Configure Color Control](images/image2022-10-18_16-3-56.png)

*   Make sure that CurrentHue and CurrentSaturation attributes are enabled:
![Attribute Selections](images/image2022-10-18_16-5-15.png)
*   Click **Back** to return to the outer panel
*   Select File → Save to save the profile you just modified
*   Exit the ZAP tool

## Step 4: Generate source and header files ##

Use the newly modified .zap file to create project files:

*   Create a folder inside **"zzz\_generated/"**, this folder will later be used by ZAP tool to create source file related to the cluster configuration of the Matter device:
    *   Create a folder with the name: **colorLight** inside **zzz\_generated**
    *   Create a second folder named **"zap-generated"** inside your newly created folder - **colorLight** 
*   Generate files:

    `$ ./scripts/tools/zap/generate.py silabs_examples/colorLight/colorLight_DataModel_config/colorLight.zap -o zzz_generated/colorLight/zap-generated/`

*   Copy the file **af-gen-event.h** from **zzz-generated/*lighting-app*/zap-generated** to **zzz-generated/*colorLight*/zap-generated** 
    

## Step 5: Integrate RGB lighting and dimming functionality ##

Copy the **sl-newLight** example project's four light control source and header files to the current project from
**~/matter/silabs_examples/sl-newLight/efr32/light_Modules/**

| File | Copy To |
| ------------------------- | -------------------------------------------------- | 
| **led\_widget\_rgb.cpp**  | ~/matter/silabs_examples/colorLight/efr32/**src**/ |
| **LightingManager.cpp**  |  ~/matter/silabs_examples/colorLight/efr32/**src**/ | 
| **led_widget_rgb.h** |  ~/matter/silabs_examples/colorLight/efr32/**include**/ |
| **LightingManager.h** | ~/matter/silabs_examples/colorLight/efr32/**include**/ |



## Step 6: Add callbacks to handle light control events ##

Modify the file **AppTask.h** inside **silabs\_examples/colorLight/efr32/include**

*   Include additional header file: **"LightingManager.h"**
*   Modify the class **AppTask** to add handlers for light control:
    
    ```cpp
    class AppTask : public BaseApplication
    {
    
    public:
        
        AppTask() = default;
        static AppTask & GetAppTask() { return sAppTask; }
        static void AppTaskMain(void * pvParameter);
        CHIP_ERROR StartAppTask();
    
    
        void ButtonEventHandler(const sl_button_t * buttonHandle, uint8_t btnAction) override;
        static void OnIdentifyStart(Identify * identify);
        static void OnIdentifyStop(Identify * identify);
    
        void PostLightActionRequest(int32_t aActor, LightingManager::Action_t aAction);
        void PostLightControlActionRequest(int32_t aActor, LightingManager::Action_t aAction, uint8_t value);
    
    private:
        static AppTask sAppTask;
    
        static void ActionInitiated(LightingManager::Action_t aAction, int32_t aActor);
        static void ActionCompleted(LightingManager::Action_t aAction);
        static void LightActionEventHandler(AppEvent * aEvent);
        static void LightControlEventHandler(AppEvent * aEvent);
        static void UpdateClusterState(intptr_t context);
        static void ButtonHandler(AppEvent * aEvent);
        CHIP_ERROR Init();
    };
    ```
    

Modify the file **AppTask.cpp** inside **silabs\_examples/colorLight/efr32/src/**, implementing functions to handle light control events.

*   Include additional header file: **"led\_widget\_rgb.h"**
*   Add a device of type RGB led to your project:
    *   Include additional header file: **"led\_widget\_rgb.h"**
    *   Add an object of type **LIGHT\_LED\_RGB**:
        
        ```cpp
        ...
        namespace {
        
        #define LIGHT_LED_RGB &sl_led_rgb_pwm
        LEDWidgetRGB sLightLED;
        
        #ifdef EMBER_AF_PLUGIN_IDENTIFY_SERVER
        ...
        ```
        
*   Modify initialization function, in which we will setup appropriate callbacks:
    
    ```cpp
    CHIP_ERROR AppTask::Init()
    {
        CHIP_ERROR err = CHIP_NO_ERROR;
    
        err = BaseApplication::Init(&gIdentify);
    
        if (err != CHIP_NO_ERROR)
        {
            EFR32_LOG("BaseApplication::Init() failed");
            appError(err);
        }
    
        err = LightMgr().Init();
        if (err != CHIP_NO_ERROR)
        {
            EFR32_LOG("LightMgr::Init() failed");
            appError(err);
        }
        LightMgr().SetCallbacks(ActionInitiated, ActionCompleted);
    
        LEDWidgetRGB::InitGpioRGB();
        sLightLED.Init(LIGHT_LED_RGB);
    
        sLightLED.Set(LightMgr().IsLightOn());
    
        return err;
    }
    ```
    
*   Implement missing callback to respond to commands, add these functions to the end of the file:
    
    ```cpp
    void AppTask::LightActionEventHandler(AppEvent * aEvent)
    {
        bool initiated = false;
        LightingManager::Action_t action;
        int32_t actor;
        CHIP_ERROR err = CHIP_NO_ERROR;
    
        if (aEvent->Type == AppEvent::kEventType_Light)
        {
            action = static_cast<LightingManager::Action_t>(aEvent->LightEvent.Action);
            actor  = aEvent->LightEvent.Actor;
        }
        else if (aEvent->Type == AppEvent::kEventType_Button)
        {
            if (LightMgr().IsLightOn())
            {
                action = LightingManager::OFF_ACTION;
    
            }
            else
            {
                action = LightingManager::ON_ACTION;
     
            }
            actor = AppEvent::kEventType_Button;
        }
        else
        {
            err = APP_ERROR_UNHANDLED_EVENT;
        }
    
        if (err == CHIP_NO_ERROR)
        {
            initiated = LightMgr().InitiateAction(actor, action);
    
            if (!initiated)
            {
                EFR32_LOG("Action is already in progress or active.");
            }
        }
    }
    void AppTask::ActionInitiated(LightingManager::Action_t aAction, int32_t aActor)
    {
        bool lightOn = aAction == LightingManager::ON_ACTION;
        EFR32_LOG("Turning light %s", (lightOn) ? "On" : "Off")
        sLightLED.Set(lightOn);
    
        if (aActor == AppEvent::kEventType_Button)
        {
            sAppTask.mSyncClusterToButtonAction = true;
        }
    }
    
    void AppTask::ActionCompleted(LightingManager::Action_t aAction)
    {
        // action has been completed on the light
        if (aAction == LightingManager::ON_ACTION)
        {
            EFR32_LOG("Light ON")
        }
        else if (aAction == LightingManager::OFF_ACTION)
        {
            EFR32_LOG("Light OFF")
        }
        if (sAppTask.mSyncClusterToButtonAction)
        {
            chip::DeviceLayer::PlatformMgr().ScheduleWork(UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));
            sAppTask.mSyncClusterToButtonAction = false;
        }
    }
    
    void AppTask::LightControlEventHandler(AppEvent * aEvent)
    {
        /* 1. Unpack the AppEvent */
        uint8_t light_action = aEvent->LightControlEvent.Action;
        uint8_t value = aEvent->LightControlEvent.Value;
    
        /* 2. Excute the control command. */
        if (light_action == LightingManager::MOVE_TO_LEVEL)
        {
            sLightLED.SetLevel(value);
            EFR32_LOG("Level set to: %d.", value);
        }
        else if (light_action == LightingManager::MOVE_TO_HUE)
        {
            sLightLED.SetHue(value);
            EFR32_LOG("Light LED hue set.");
        }
        else if (light_action == LightingManager::MOVE_TO_SAT)
        {
            sLightLED.SetSaturation(value);
            EFR32_LOG("Light LED saturation set.");
        }
    }
    void AppTask::PostLightActionRequest(int32_t aActor, LightingManager::Action_t aAction)
    {
        AppEvent event;
        event.Type              = AppEvent::kEventType_Light;
        event.LightEvent.Actor  = aActor;
        event.LightEvent.Action = aAction;
        event.Handler           = LightActionEventHandler;
        PostEvent(&event);
    }
    void AppTask::PostLightControlActionRequest(int32_t aActor, LightingManager::Action_t aAction, uint8_t value)
    {
        AppEvent light_event                    = {};
        light_event.Type                        = AppEvent::kEventType_Light;
        light_event.LightControlEvent.Actor     = aActor;
        light_event.LightControlEvent.Action    = aAction;
        light_event.LightControlEvent.Value     = value;
        light_event.Handler                     = LightControlEventHandler;
        PostEvent(&light_event);
    }
    void AppTask::UpdateClusterState(intptr_t context)
    {
        uint8_t newValue = LightMgr().IsLightOn();
        // write the new on/off value
        EmberAfStatus status = OnOffServer::Instance().setOnOffValue(1, newValue, false);
        if (status != EMBER_ZCL_STATUS_SUCCESS)
        {
            EFR32_LOG("ERR: updating on/off %x", status);
        }
    }
    ```


## Step 7: Add additional light control data ##

Modify the file **AppEvent.h** inside **silabs\_examples/colorLight/efr32/include/**, add a **LightControlEvent** struct to the union of struct AppEvent in addition to the basic on/off **LightEvent**:

```cpp
struct AppEvent
{
	...
    union
    {
        struct
        {
            uint8_t Action;
        } ButtonEvent;
        struct
        {
            void * Context;
        } TimerEvent;
        struct
        {
            uint8_t Action;
            int32_t Actor;
        } LightEvent;
        struct
        {
            uint8_t Action;
            int32_t Actor;
            uint8_t Value;
        } LightControlEvent;
    };
	...
};
```

## Step 8: Implement code to handle Attribute changes properly ##

Modify the file **ZclCallbacks.cpp** inside **silabs\_examples/colorLight/efr32/src/**

*   Include additional header file: **"LightingManager.h"**
*   Modify **MatterPostAttributeChangeCallback** to use the Lighting Manager on our board for each type of command:

```cpp
void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    ClusterId clusterId     = attributePath.mClusterId;
    AttributeId attributeId = attributePath.mAttributeId;
    EndpointId endpoint     = attributePath.mEndpointId;
    LightingManager::Action_t action_type = LightingManager::IGNORE_ACTION;

    ChipLogProgress(Zcl, "Cluster callback: " ChipLogFormatMEI, ChipLogValueMEI(clusterId));

    if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::OnOff::Id)
    {
        LightMgr().InitiateAction(AppEvent::kEventType_Light, *value ? LightingManager::ON_ACTION : LightingManager::OFF_ACTION);
    }
    else if (clusterId == LevelControl::Id)
    {
       if (attributeId == LevelControl::Attributes::CurrentLevel::Id)
       {
          action_type = LightingManager::MOVE_TO_LEVEL; 
       }

       LightMgr().InitiateActionLight(AppEvent::kEventType_Light, action_type, endpoint, *value);
    }
    else if (clusterId == ColorControl::Id)
    {
        if (attributeId == ColorControl::Attributes::CurrentHue::Id)
        {
            action_type = LightingManager::MOVE_TO_HUE;
        } 
        else if (attributeId == ColorControl::Attributes::CurrentSaturation::Id) 
        {
            action_type = LightingManager::MOVE_TO_SAT;
        }

        LightMgr().InitiateActionLight(AppEvent::kEventType_Light, action_type, endpoint, *value);
    }
}
```

## Step 9: Set up Device Attestation Credentials Provider for your Matter End Device ##

Modify the file **main.cpp** inside **silabs\_examples/colorLight/efr32/src/**

*   Include additional header file: 

```cpp
#include <DeviceInfoProviderImpl.h>
#include <app/server/Server.h>
#include <credentials/DeviceAttestationCredsProvider.h>

#ifdef EFR32_ATTESTATION_CREDENTIALS
#include <examples/platform/efr32/EFR32DeviceAttestationCreds.h>
#else
#include <credentials/examples/DeviceAttestationCredsExample.h>
#endif
```

*   Use additional namespace:

```cpp
...
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;

#define UNUSED_PARAMETER(a) (a = a)
...
```

*   Add a global variable of type DeviceInfoProviderImpl, this will later be used to set the device info provider

```cpp
static chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
```

*   Modify the main() function to set up the device attestation credential provider:

```cpp
int main(void)
{
    init_efrPlatform();
    if (EFR32MatterConfig::InitMatter(BLE_DEV_NAME) != CHIP_NO_ERROR)
        appError(CHIP_ERROR_INTERNAL);
    gExampleDeviceInfoProvider.SetStorageDelegate(&chip::Server::GetInstance().GetPersistentStorage());
    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    chip::DeviceLayer::PlatformMgr().LockChipStack();
    // Initialize device attestation config
#ifdef EFR32_ATTESTATION_CREDENTIALS
    SetDeviceAttestationCredentialsProvider(EFR32::GetEFR32DacProvider());
#else
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

    chip::DeviceLayer::PlatformMgr().UnlockChipStack();
    EFR32_LOG("Starting App Task");

    if (AppTask::GetAppTask().StartAppTask() != CHIP_NO_ERROR)
        appError(CHIP_ERROR_INTERNAL);

    EFR32_LOG("Starting FreeRTOS scheduler");
    sl_system_kernel_start();

    // Should never get here.
    chip::Platform::MemoryShutdown();
    EFR32_LOG("vTaskStartScheduler() failed");
    appError(CHIP_ERROR_INTERNAL);
}
```

## Step 10: Modify the GN build file ##

Modify the file **BUILD.gn** inside **_silabs\_examples/colorLight/_efr32** to change the paths from the template and include the source file changes, **"src/led_widget_rgb.cpp"** and **"src/LightingManager.cpp"**:  

```cpp
...
efr32_project_dir = "${chip_root}/silabs_examples/colorLight/efr32"
...
efr32_executable("colorLight_app") {
  output_name = "efr32-colorLight.out"
  include_dirs = [ "include" ]
  defines = []

  sources = [
    "${examples_plat_dir}/BaseApplication.cpp",
    "${examples_plat_dir}/efr32_utils.cpp",
    "${examples_plat_dir}/heap_4_silabs.c",
    "${examples_plat_dir}/init_efrPlatform.cpp",
    "${examples_plat_dir}/matter_config.cpp",
    "src/led_widget_rgb.cpp",
    "src/LightingManager.cpp",
    "src/AppTask.cpp",
    "src/ZclCallbacks.cpp",
    "src/main.cpp",
  ]
...
  deps = [
    ":sdk",
    "${chip_root}/examples/providers:device_info_provider",
    "${chip_root}/silabs_examples/colorLight/colorLight_DataModel_config",
    "${chip_root}/src/lib",
    "${chip_root}/src/setup_payload",
  ]
...
    if (show_qr_code) {
      defines += [ "QR_CODE_ENABLED" ]

      deps += [ "${chip_root}/examples/common/QRCode" ]
    }
    sources += [ "light_modules/led_widget_rgb.cpp" ]
    defines += [ "RGB_LED_ENABLED" ]
  }
...
group("efr32") {
  deps = [ ":colorLight_app" ]
}
...
```

## Step 11: Build the project ##

From the **matter** folder, run:

```plain
$ ./scripts/examples/gn_efr32_example.sh ./silabs_examples/colorLight/efr32/ ./out/colorLight BRD4166A
```

## Step 12: Flash the firmware ##

Flash the binary file located at **out/colorLight/BRD4166A/efr32-colorLight.s37** to your Thunderboard sense 2 board

## Step 13: Commission and control the MAD ##

Once the firmware has been flashed onto your Matter Accessory device you can commission it from the Matter Hub using the commands provided in the Raspberry Pi image:

| Command | Usage | Note |
| ---------------------------| ----------------- | --------------------- |
| mattertool startThread | Starts the thread network on the OTBR |    |
| mattertool bleThread | Starts commissioning of a MAD using chip-tool |   |
| mattertool -h | Gets the Node ID of your MAD |    |
| mattertool on | Sends an **on** command to the MAD using chip-tool |    |
| mattertool off | Sends an **off** command to the MAD using chip-tool |   |
| mattertool levelcontrol move-to-level {desired\_level} 0 1 1 {node\_ID} 1 | Sets the brightness level (color intensity) | Level range: 0 - 254 |
| mattertool colorcontrol move-to-saturation {desired\_saturation} 0 1 1 {node\_ID} 1 | Sets the saturation value | Value range: 0 - 254 |
| mattertool colorcontrol move-to-hue {desired\_hue} 0 0 1 1 {node\_ID} 1  | Sets the Hue value | Value range: 0 - 254 |

The HSV (Hue/Saturation/Intensity) colorspace as it relates to RGB LEDs is a complicated subject, and converting to an 8 bit range adds to that. You can find a lot of material online regarding this system; [this Wikipedia article](https://en.wikipedia.org/wiki/HSL_and_HSV) is a good place to begin.

Here are some fun values to test out using the mattertool commands:

| Hue | Saturation | Level |
| :-: | :-: | :-: |
| 45 | 5 | 10 |
| 90 | 54 | 10 |
| 135 | 243 | 210 |
| 175 | 5 | 10 |
| 220 | 24 | 90 |

For example, if your Node ID is **22913**, issue the following 3 commands:  

`$ mattertool colorcontrol move-to-hue 220 0 0 1 1 22913 1`  

`$ mattertool colorcontrol move-to-saturation 24 0 1 1 22913 1`  

`$ mattertool levelcontrol move-to-level 90 0 1 1 22913 1`
