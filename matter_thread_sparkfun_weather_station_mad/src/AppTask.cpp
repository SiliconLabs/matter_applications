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

#define APP_FUNCTION_BUTTON 0
#define APP_SENSOR_SWITCH 1

using namespace chip;
using namespace ::chip::DeviceLayer;
using namespace chip::TLV;

#include "weather-station-cluster.h"
#include "sl_sleeptimer.h"

#include "sl_i2cspm_instances.h"
#include "sl_si70xx.h"

#include "sparkfun_weather_station_rainfall.h"
#include "sparkfun_weather_station_wind_direction.h"
#include "sparkfun_weather_station_wind_speed.h"
#include "sparkfun_weather_station_wind_speed_config.h"

#define MEASUREMENT_PERIOD SPARKFUN_WINDSPEED_CHECK_PERIOD_MS
#define RAINFALL_RESET_PERIOD_MS 600000

sl_sleeptimer_timer_handle_t timer_handle;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::DeviceLayer::Silabs::GetPlatform().SetButtonsCb(AppTask::ButtonEventHandler);

    err = BaseApplication::Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("BaseApplication::Init() failed");
        appError(err);
    }


    sl_si70xx_init(sl_i2cspm_inst0, SI7021_ADDR);

    sl_status_t status;

    sparkfun_weatherstation_rainfall_init();
    status = sparkfun_weatherstation_winddirection_init();
    sparkfun_weatherstation_windspeed_init();

    if (status != SL_STATUS_OK)
    {
        err = CHIP_ERROR_WELL_UNINITIALIZED;
    }

    sl_sleeptimer_start_periodic_timer_ms(&timer_handle, MEASUREMENT_PERIOD, AppTask::TimerCallback, (void*) NULL, 0, 0);

    return err;
}

CHIP_ERROR AppTask::StartAppTask()
{
    return BaseApplication::StartAppTask(AppTaskMain);
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;
    QueueHandle_t sAppEventQueue = *(static_cast<QueueHandle_t *>(pvParameter));

    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("AppTask.Init() failed");
        appError(err);
    }

#if !(defined(CHIP_DEVICE_CONFIG_ENABLE_SED) && CHIP_DEVICE_CONFIG_ENABLE_SED)
    sAppTask.StartStatusLEDTimer();
#endif

    SILABS_LOG("App Task started");

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, portMAX_DELAY);
        while (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }
    }
}

void AppTask::TimerCallback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
    AppEvent sensor_event           = {};
     sensor_event.Type               = AppEvent::kEventType_Timer;
     sensor_event.TimerEvent.Context = data;
    sensor_event.Handler = AppTask::ActionTriggered;
    sAppTask.PostEvent(&sensor_event);
}


void AppTask::ActionTriggered(AppEvent * aEvent)
{
    float rain, windspeed, winddirection;
    static float lastrain = 0;
    static uint32_t rainless_periods = 0;
    uint32_t rh_data;
    int32_t temp_data;

    //read sensors
    sparkfun_weatherstation_rainfall_get_rainfall_amount(&rain);
    sparkfun_weatherstation_windspeed_get(&windspeed);
    sparkfun_weatherstation_winddirection_read_direction(&winddirection);

    //check if rain has stopped to reset rainfall count
    if (rain > lastrain){
      //rain has increased further, so reset rainless_periods
      rainless_periods = 0;
    } else {
      rainless_periods++;
      if (rainless_periods >= RAINFALL_RESET_PERIOD_MS / MEASUREMENT_PERIOD){
        sparkfun_weatherstation_rainfall_reset_rainfall_count();
        //read new value (should be 0)
        sparkfun_weatherstation_rainfall_get_rainfall_amount(&rain);
      }
    }
    lastrain = rain;


    sl_si70xx_measure_rh_and_temp(sl_i2cspm_inst0, SI7021_ADDR, &rh_data, &temp_data);

    temp_data/=10;
    rh_data/=10;

    //update attributes
    chip::DeviceLayer::PlatformMgr().LockChipStack();
    UpdateRainfall(1,rain);
    UpdateWindSpeed(1,windspeed);
    UpdateWindDirection(1,static_cast<uint32_t>(winddirection));
    UpdateTemperature(1, static_cast<int16_t>(temp_data));
    UpdateHumidity(2, static_cast<uint16_t>(rh_data));
    chip::DeviceLayer::PlatformMgr().UnlockChipStack();

}



void AppTask::ButtonEventHandler(uint8_t button, uint8_t btnAction)
{

    AppEvent button_event           = {};
    button_event.Type               = AppEvent::kEventType_Button;
    button_event.ButtonEvent.Action = btnAction;

    if (button == APP_SENSOR_SWITCH)
    {
        button_event.Handler = AppTask::ActionTriggered;
        sAppTask.PostEvent(&button_event);
    }
    else if (button == APP_FUNCTION_BUTTON)
    {
        button_event.Handler = BaseApplication::ButtonHandler;
        sAppTask.PostEvent(&button_event);
    }
}
