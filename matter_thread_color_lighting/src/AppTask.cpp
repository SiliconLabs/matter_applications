/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AppTask.h"
#include "AppConfig.h"
#include "AppEvent.h"

#include "LEDWidget.h"

#include <app/clusters/on-off-server/on-off-server.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>

#include <assert.h>

#include <platform/silabs/platformAbstraction/SilabsPlatform.h>

#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#include <lib/support/CodeUtils.h>

#include <platform/CHIPDeviceLayer.h>

//#include "sl_simple_rgb_pwm_led_instances.h"
//#include "sl_simple_rgb_pwm_led.h"

#include "mikroe_rgb.h"
#include "sl_sleeptimer.h"
#include "sl_i2cspm_instances.h"
//#include "app_log.h"

#ifdef SL_CATALOG_SIMPLE_LED_LED1_PRESENT
#define LIGHT_LED 1
#else
#define LIGHT_LED 0
#endif

#define APP_FUNCTION_BUTTON 0
#define APP_LIGHT_SWITCH 1

using namespace chip;
using namespace chip::app;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceLayer::Silabs;


//namespace {
//LEDWidget sLightLED;
//}

using namespace chip::TLV;
using namespace ::chip::DeviceLayer;

AppTask AppTask::sAppTask;

static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void led_rgb_update(uint8_t level, uint8_t hue, uint8_t sat);

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::DeviceLayer::Silabs::GetPlatform().SetButtonsCb(AppTask::ButtonEventHandler);

#ifdef DISPLAY_ENABLED
    GetLCD().Init((uint8_t *) "Lighting-App");
#endif

    err = BaseApplication::Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("BaseApplication::Init() failed");
        appError(err);
    }

    err = LightMgr().Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("LightMgr::Init() failed");
        appError(err);
    }

    app_i2c_instance = sl_i2cspm_mikroe;
    mikroe_rgbdriver_init(app_i2c_instance);

    LightMgr().SetCallbacks(ActionInitiated, ActionCompleted);
    led_rgb_update(LightMgr().GetCurrentLevel(),
                   LightMgr().GetCurrentColorHue(),
                   LightMgr().GetCurrentColorSaturation());

//    sLightLED.Init(LIGHT_LED);
//    sLightLED.Set(LightMgr().IsLightOn());

// Update the LCD with the Stored value. Show QR Code if not provisioned
#ifdef DISPLAY_ENABLED
    GetLCD().WriteDemoUI(LightMgr().IsLightOn());
#ifdef QR_CODE_ENABLED
#ifdef SL_WIFI
    if (!ConnectivityMgr().IsWiFiStationProvisioned())
#else
    if (!ConnectivityMgr().IsThreadProvisioned())
#endif /* !SL_WIFI */
    {
        GetLCD().ShowQRCode(true);
    }
#endif // QR_CODE_ENABLED
#endif

    return err;
}

CHIP_ERROR AppTask::StartAppTask()
{
    return BaseApplication::StartAppTask(AppTaskMain);
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;
    osMessageQueueId_t sAppEventQueue = *(static_cast<osMessageQueueId_t *>(pvParameter));

    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("AppTask.Init() failed");
        appError(err);
    }

#if !(defined(CHIP_CONFIG_ENABLE_ICD_SERVER) && CHIP_CONFIG_ENABLE_ICD_SERVER)
    sAppTask.StartStatusLEDTimer();
#endif

    SILABS_LOG("App Task started");

    while (true)
    {
        osStatus_t eventReceived = osMessageQueueGet(sAppEventQueue, &event, NULL, osWaitForever);
        while (eventReceived == osOK)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = osMessageQueueGet(sAppEventQueue, &event, NULL, 0);
        }
    }
}

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
        action = (LightMgr().IsLightOn()) ? LightingManager::OFF_ACTION : LightingManager::ON_ACTION;
        actor  = AppEvent::kEventType_Button;
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
            SILABS_LOG("Action is already in progress or active.");
        }
    }
}

void AppTask::ButtonEventHandler(uint8_t button, uint8_t btnAction)
{
    AppEvent button_event           = {};
    button_event.Type               = AppEvent::kEventType_Button;
    button_event.ButtonEvent.Action = btnAction;

    if (button == APP_LIGHT_SWITCH && btnAction == static_cast<uint8_t>(SilabsPlatform::ButtonAction::ButtonPressed))
    {
        button_event.Handler = LightActionEventHandler;
        AppTask::GetAppTask().PostEvent(&button_event);
    }
    else if (button == APP_FUNCTION_BUTTON)
    {
        button_event.Handler = BaseApplication::ButtonHandler;
        AppTask::GetAppTask().PostEvent(&button_event);
    }
}

void AppTask::ActionInitiated(LightingManager::Action_t aAction, int32_t aActor)
{
  uint8_t level, hue, sat;
    // Action initiated, update the light led
//    bool lightOn = aAction == LightingManager::ON_ACTION;
//    SILABS_LOG("Turning light %s", (lightOn) ? "On" : "Off")

//    sLightLED.Set(lightOn);
    switch (aAction) {
      case LightingManager::MOVE_TO_LEVEL_ACTION:
        level = LightMgr().GetCurrentLevel();
        hue = LightMgr().GetCurrentColorHue();
        sat = LightMgr().GetCurrentColorSaturation();
        led_rgb_update(level, hue, sat);
        SILABS_LOG("Move to level: %d", level);
        break;
      case LightingManager::MOVE_TO_HUE_SAT_ACTION:
        level = LightMgr().GetCurrentLevel();
        hue = LightMgr().GetCurrentColorHue();
        sat = LightMgr().GetCurrentColorSaturation();
        led_rgb_update(level, hue, sat);
        SILABS_LOG("Move to hue and saturation: %d, %d", hue, sat);
        break;
      case LightingManager::ON_ACTION:
        level = LightMgr().GetCurrentLevel();
        hue = LightMgr().GetCurrentColorHue();
        sat = LightMgr().GetCurrentColorSaturation();
        led_rgb_update(level, hue, sat);
        break;

      case LightingManager::OFF_ACTION:
//        sl_led_turn_off((sl_led_t *)&sl_simple_rgb_pwm_led_rgb_led0);
        mikroe_rgbdriver_shut_down();

        break;

      default:
        break;
    }

#ifdef DISPLAY_ENABLED
    sAppTask.GetLCD().WriteDemoUI(lightOn);
#endif

    if (aActor == AppEvent::kEventType_Button)
    {
        sAppTask.mSyncClusterToButtonAction = true;
    }
}

void AppTask::ActionCompleted(LightingManager::Action_t aAction)
{
    // action has been completed bon the light
    if (aAction == LightingManager::ON_ACTION)
    {
        SILABS_LOG("Light ON")
    }
    else if (aAction == LightingManager::OFF_ACTION)
    {
        SILABS_LOG("Light OFF")
    }

    if (sAppTask.mSyncClusterToButtonAction)
    {
        chip::DeviceLayer::PlatformMgr().ScheduleWork(UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));
        sAppTask.mSyncClusterToButtonAction = false;
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

void AppTask::UpdateClusterState(intptr_t context)
{
    uint8_t newValue = LightMgr().IsLightOn();

    // write the new on/off value
    Protocols::InteractionModel::Status status = OnOffServer::Instance().setOnOffValue(1, newValue, false);

    if (status != Protocols::InteractionModel::Status::Success)
    {
        SILABS_LOG("ERR: updating on/off %x", to_underlying(status));
    }
}

static void hsv_to_rgb
(
  uint8_t hue, uint8_t saturation, uint8_t level,
  uint8_t *R8, uint8_t *G8, uint8_t *B8
)
{
  uint32_t min32, hue32, delta32, sat32, level32;

  uint32_t R32, G32, B32;

  // algorithm taken from wikipedia
  // http://en.wikipedia.org/wiki/CIE_1931_color_space for more details

  // note:  hue and saturation are 0..254.  level (i.e. value for HSV) is
  // 0..255.  But most of these computations assume 0..1 for saturation and
  // value, and 0..360 for hue.  This will be a little tricky to compute RGB
  // using fixed point math and not lose any bits of significance.

  // first switch to 32 bit mode.
  level32 = 255;
  sat32 = (uint32_t) saturation;

  min32 = level32 * (254 - sat32);
  min32 = min32 / 254;
  delta32 = level32 - min32;
  hue32 = ((uint32_t) hue);  // need to map it to 0..6.  really is 0..254.

  // formula is X = C * { 1 - ( H mod2 - 1) }.  Because the numbers don't line
  // up, we need to do this with if's.
  // The ranges are 0, 42, 84, 127, 169, 211, 254.
  if (hue < 43) {
    R32 = level32; // 0..254
    // convert 0..42 to 0..delta32
    G32 = hue32 * delta32;
    G32 = G32 / 42;
    G32 = G32 + min32;
    B32 = min32;
  }
  else if (hue < 85)
  {
    hue32 -= 42;
    // convert 0..42 to delta32..0
    R32 = (42 - hue32) * delta32;
    R32 = R32 / 42;
    R32 = R32 + min32;
    G32 = level32;
    B32 = min32;
  }
  else if (hue < 128)
  {
    hue32 -= 84;
    R32 = min32;
    G32 = level32;
    // convert 0..43 to 0..delta32
    B32 = hue32 * delta32;
    B32 = B32 / 43;
    B32 = B32 + min32;
  }
  else if (hue < 170)
  {
    hue32 -= 127;
    R32 = min32;
    // convert 0..42 to delta32..0
    G32 = (42 - hue32) * delta32;
    G32 = G32 / 42;
    G32 = G32 + min32;
    B32 = level32;
  }
  else if (hue < 212)
  {
    hue32 -= 169;
    // convert 0..42 to 0..delta32
    R32 = hue32 * delta32;
    R32 = R32 / 42;
    R32 = R32 + min32;
    G32 = min32;
    B32 = level32;
  }
  else
  { //hue is 212..254
    hue32 -= 211;
    R32 = level32;
    G32 = min32;
    // convert 0..43 to delta32..0
    B32 = (43 - hue32) * delta32;
    B32 = B32 / 42;
    B32 = B32 + min32;
  }

  R32 *= 256;
  *R8 = (R32 / 254) * level / 256;
  G32 *= 256;
  *G8 = (G32 / 254) * level / 256;
  B32 *= 256;
  *B8 = (B32 / 254) * level / 256;
}

static void led_rgb_update(uint8_t level, uint8_t hue, uint8_t sat)
{
  uint8_t r, g, b;

  hsv_to_rgb(hue, sat, level,
             &r, &g, &b);
  SILABS_LOG("hue: %d, sat: %d, R : %d ; G : %d ; B : %d\n",hue, sat, r,g,b);
  mikroe_rgbdriver_set_rgb_color(r, g, b);
}
