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

#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>

#include <assert.h>

#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#include <lib/support/CodeUtils.h>

#include <platform/CHIPDeviceLayer.h>
#include <platform/silabs/platformAbstraction/SilabsPlatform.h>

#include "air-quality-sensor-manager.h"
extern "C" {
#include "CoDetector.h"
}
#if defined(SL_CATALOG_SIMPLE_LED_LED1_PRESENT)
#define SENSOR_LED            1
#else
#define SENSOR_LED            0
#endif

#define APP_FUNCTION_BUTTON   0
#define APP_SENSOR_SWITCH     1

#define READING_INTERVAL_MSEC 800000
sl_sleeptimer_timer_handle_t app_timer_handle;
chip::app::Clusters::AirQualitySensorManager *airQualitySensor_instance;

using namespace chip;
using namespace ::chip::DeviceLayer;

namespace {
LEDWidget sSensorLED;
} // namespace

using namespace chip::TLV;
using namespace ::chip::DeviceLayer;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::Init()
{
  CHIP_ERROR err = CHIP_NO_ERROR;
  chip::DeviceLayer::Silabs::GetPlatform().SetButtonsCb(
    AppTask::ButtonEventHandler);
#ifdef DISPLAY_ENABLED
  GetLCD().Init((uint8_t *) SENSOR_NAME);
#endif

  err = BaseApplication::Init();
  if (err != CHIP_NO_ERROR) {
    SILABS_LOG("BaseApplication::Init() failed");
    appError(err);
  }

  sSensorLED.Init(SENSOR_LED);
  sSensorLED.Set(false);

  if (CoDetector_init() == SL_STATUS_OK) {
    SILABS_LOG("CoDetector_init() success!");
  } else {
    SILABS_LOG("CoDetector_init() failed!");
  }

  chip::app::Clusters::AirQualitySensorManager::InitInstance();
  airQualitySensor_instance =
    chip::app::Clusters::AirQualitySensorManager::GetInstance();

  sl_sleeptimer_start_periodic_timer(&app_timer_handle,
                                     READING_INTERVAL_MSEC,
                                     AppTask::TimerCallback,
                                     (void *) NULL,
                                     0,
                                     0);

// Update the LCD with the Stored value. Show QR Code if not provisioned
#ifdef DISPLAY_ENABLED
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

void AppTask::AppTaskMain(void *pvParameter)
{
  AppEvent event;
  QueueHandle_t sAppEventQueue = *(static_cast<QueueHandle_t *>(pvParameter));

  CHIP_ERROR err = sAppTask.Init();
  if (err != CHIP_NO_ERROR) {
    SILABS_LOG("AppTask.Init() failed");
    appError(err);
  }

#if !(defined(CHIP_DEVICE_CONFIG_ENABLE_SED) && CHIP_DEVICE_CONFIG_ENABLE_SED)
  sAppTask.StartStatusLEDTimer();
#endif

  SILABS_LOG("App Task started");

  while (true)
  {
    BaseType_t eventReceived = xQueueReceive(sAppEventQueue,
                                             &event,
                                             portMAX_DELAY);
    while (eventReceived == pdTRUE)
    {
      sAppTask.DispatchEvent(&event);
      eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
    }
  }
}

void AppTask::ButtonEventHandler(uint8_t button, uint8_t btnAction)
{
  AppEvent button_event = {};
  button_event.Type = AppEvent::kEventType_Button;
  button_event.ButtonEvent.Action = btnAction;

  if (button == APP_FUNCTION_BUTTON) {
    button_event.Handler = BaseApplication::ButtonHandler;
    sAppTask.PostEvent(&button_event);
  }
}

void AppTask::SetAppLED(bool state)
{
  sSensorLED.Set(state);
}

void AppTask::TimerCallback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  AppEvent sensor_event = {};
  sensor_event.Type = AppEvent::kEventType_Timer;
  sensor_event.TimerEvent.Context = data;
  sensor_event.Handler = AppTask::ActionTriggered;
  sAppTask.PostEvent(&sensor_event);
}

void AppTask::ActionTriggered(AppEvent *aEvent)
{
  uint16_t new_value = CoDetector_read();
  chip::DeviceLayer::PlatformMgr().LockChipStack();
  airQualitySensor_instance->OnCarbonMonoxideMeasurementChangeHandler(float(
                                                                        new_value));
  chip::DeviceLayer::PlatformMgr().UnlockChipStack();
}
