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
#if defined(ENABLE_CHIP_SHELL)
#include "EventHandlerLibShell.h"
#endif // ENABLE_CHIP_SHELL

#include "LEDWidget.h"

#ifdef DISPLAY_ENABLED
#include "lcd.h"
#ifdef QR_CODE_ENABLED
#include "qrcodegen.h"
#endif // QR_CODE_ENABLED
#endif // DISPLAY_ENABLED

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>

#include <app/clusters/door-lock-server/door-lock-server.h>
//< WW2023: Add require clusters for light
#include <app/clusters/on-off-server/on-off-server.h>
//>
#include <app/clusters/identify-server/identify-server.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>

#include <assert.h>

#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#include <lib/support/CodeUtils.h>

#include <platform/CHIPDeviceLayer.h>
#define SYSTEM_STATE_LED 0
#define LOCK_STATE_LED 1

//< WW2023: Add define for light LED support; changing it from LED1 > LED0
#define LIGHT_LED 0

#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
#define APP_FUNCTION_BUTTON &sl_button_btn0
#define APP_LOCK_SWITCH &sl_button_btn1
#endif // SL_CATALOG_SIMPLE_BUTTON_PRESENT

using chip::app::Clusters::DoorLock::DlLockState;
using chip::app::Clusters::DoorLock::OperationErrorEnum;
using chip::app::Clusters::DoorLock::OperationSourceEnum;

using namespace chip;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceLayer::Internal;
using namespace EFR32DoorLock::LockInitParams;

namespace {
LEDWidget sLockLED;
// WW2023: Add LEDWidget for light code
LEDWidget sLightLED;

EmberAfIdentifyEffectIdentifier sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;
} // namespace
/**********************************************************
 * Identify Callbacks
 *********************************************************/

namespace {
void OnTriggerIdentifyEffectCompleted(chip::System::Layer * systemLayer, void * appState)
{
    ChipLogProgress(Zcl, "Trigger Identify Complete");
    sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;

#if CHIP_DEVICE_CONFIG_ENABLE_SED == 1
    AppTask::GetAppTask().StopStatusLEDTimer();
#endif
}

void OnTriggerIdentifyEffect(Identify * identify)
{
    sIdentifyEffect = identify->mCurrentEffectIdentifier;

    if (identify->mCurrentEffectIdentifier == EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE)
    {
        ChipLogProgress(Zcl, "IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE - Not supported, use effect varriant %d",
                        identify->mEffectVariant);
        sIdentifyEffect = static_cast<EmberAfIdentifyEffectIdentifier>(identify->mEffectVariant);
    }

#if CHIP_DEVICE_CONFIG_ENABLE_SED == 1
    AppTask::GetAppTask().StartStatusLEDTimer();
#endif

    switch (sIdentifyEffect)
    {
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BLINK:
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BREATHE:
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_OKAY:
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(5), OnTriggerIdentifyEffectCompleted,
                                                           identify);
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_FINISH_EFFECT:
        (void) chip::DeviceLayer::SystemLayer().CancelTimer(OnTriggerIdentifyEffectCompleted, identify);
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectCompleted,
                                                           identify);
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT:
        (void) chip::DeviceLayer::SystemLayer().CancelTimer(OnTriggerIdentifyEffectCompleted, identify);
        sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;
        break;
    default:
        ChipLogProgress(Zcl, "No identifier effect");
    }
}

Identify gIdentify = {
    chip::EndpointId{ 1 },
    AppTask::GetAppTask().OnIdentifyStart,
    AppTask::GetAppTask().OnIdentifyStop,
    EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LED,
    OnTriggerIdentifyEffect,
};

} // namespace

using namespace chip::TLV;
using namespace ::chip::DeviceLayer;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

#ifdef DISPLAY_ENABLED
    GetLCD().Init((uint8_t *) "Lock-App", true);
#endif

    err = BaseApplication::Init(&gIdentify);
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("BaseApplication::Init() failed");
        appError(err);
    }

#if defined(ENABLE_CHIP_SHELL)
    err = RegisterLockEvents();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("RegisterLockEvents() failed");
        appError(err);
    }
#endif // ENABLE_CHIP_SHELL

    // Initial lock state
    chip::app::DataModel::Nullable<chip::app::Clusters::DoorLock::DlLockState> state;
    chip::EndpointId endpointId{ 1 };
    chip::DeviceLayer::PlatformMgr().LockChipStack();
    chip::app::Clusters::DoorLock::Attributes::LockState::Get(endpointId, state);

    uint8_t numberOfCredentialsPerUser = 0;
    if (!DoorLockServer::Instance().GetNumberOfCredentialsSupportedPerUser(endpointId, numberOfCredentialsPerUser))
    {
        ChipLogError(Zcl,
                     "Unable to get number of credentials supported per user when initializing lock endpoint, defaulting to 5 "
                     "[endpointId=%d]",
                     endpointId);
        numberOfCredentialsPerUser = 5;
    }

    uint16_t numberOfUsers = 0;
    if (!DoorLockServer::Instance().GetNumberOfUserSupported(endpointId, numberOfUsers))
    {
        ChipLogError(Zcl,
                     "Unable to get number of supported users when initializing lock endpoint, defaulting to 10 [endpointId=%d]",
                     endpointId);
        numberOfUsers = 10;
    }

    uint8_t numberOfWeekdaySchedulesPerUser = 0;
    if (!DoorLockServer::Instance().GetNumberOfWeekDaySchedulesPerUserSupported(endpointId, numberOfWeekdaySchedulesPerUser))
    {
        ChipLogError(
            Zcl,
            "Unable to get number of supported weekday schedules when initializing lock endpoint, defaulting to 10 [endpointId=%d]",
            endpointId);
        numberOfWeekdaySchedulesPerUser = 10;
    }

    uint8_t numberOfYeardaySchedulesPerUser = 0;
    if (!DoorLockServer::Instance().GetNumberOfYearDaySchedulesPerUserSupported(endpointId, numberOfYeardaySchedulesPerUser))
    {
        ChipLogError(
            Zcl,
            "Unable to get number of supported yearday schedules when initializing lock endpoint, defaulting to 10 [endpointId=%d]",
            endpointId);
        numberOfYeardaySchedulesPerUser = 10;
    }

    uint8_t numberOfHolidaySchedules = 0;
    if (!DoorLockServer::Instance().GetNumberOfHolidaySchedulesSupported(endpointId, numberOfHolidaySchedules))
    {
        ChipLogError(
            Zcl,
            "Unable to get number of supported holiday schedules when initializing lock endpoint, defaulting to 10 [endpointId=%d]",
            endpointId);
        numberOfHolidaySchedules = 10;
    }

    chip::DeviceLayer::PlatformMgr().UnlockChipStack();

    err = LockMgr().Init(state,
                         ParamBuilder()
                             .SetNumberOfUsers(numberOfUsers)
                             .SetNumberOfCredentialsPerUser(numberOfCredentialsPerUser)
                             .SetNumberOfWeekdaySchedulesPerUser(numberOfWeekdaySchedulesPerUser)
                             .SetNumberOfYeardaySchedulesPerUser(numberOfYeardaySchedulesPerUser)
                             .SetNumberOfHolidaySchedules(numberOfHolidaySchedules)
                             .GetLockParam());

    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("LockMgr().Init() failed");
        appError(err);
    }

    LockMgr().SetCallbacks(ActionInitiated, ActionCompleted);

    sLockLED.Init(LOCK_STATE_LED);
    sLockLED.Set(state.Value() == DlLockState::kUnlocked);

    //< WW2023: Add light init code; use new callbacks
    err = LightMgr().Init();
    if (err != CHIP_NO_ERROR)
    {
        SILABS_LOG("LightMgr::Init() failed");
        appError(err);
    }
    LightMgr().SetCallbacks(ActionInitiatedLight, ActionCompletedLight);
    sLightLED.Init(LIGHT_LED);
    sLightLED.Set(LightMgr().IsLightOn());
    //>

    // Update the LCD with the Stored value. Show QR Code if not provisioned
#ifdef DISPLAY_ENABLED
    // WW2023: Add dual state support
    GetLCD().WriteDemoUI(state.Value() != DlLockState::kUnlocked, true);
    GetLCD().WriteDemoUI(LightMgr().IsLightOn(), false);
#ifdef QR_CODE_ENABLED
#ifdef SL_WIFI
    if (!ConnectivityMgr().IsWiFiStationProvisioned())
#else
    if (!ConnectivityMgr().IsThreadProvisioned())
#endif /* !SL_WIFI */
    {
        GetLCD().ShowQRCode(true, true);
    }
#endif // QR_CODE_ENABLED
#endif

    chip::DeviceLayer::PlatformMgr().ScheduleWork(UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));

    ConfigurationMgr().LogDeviceConfig();

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

    // Users and credentials should be checked once from nvm flash on boot
    LockMgr().ReadConfigValues();

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

void AppTask::OnIdentifyStart(Identify * identify)
{
    ChipLogProgress(Zcl, "onIdentifyStart");

#if CHIP_DEVICE_CONFIG_ENABLE_SED == 1
    sAppTask.StartStatusLEDTimer();
#endif
}

void AppTask::OnIdentifyStop(Identify * identify)
{
    ChipLogProgress(Zcl, "onIdentifyStop");

#if CHIP_DEVICE_CONFIG_ENABLE_SED == 1
    sAppTask.StopStatusLEDTimer();
#endif
}

void AppTask::LockActionEventHandler(AppEvent * aEvent)
{
    bool initiated = false;
    LockManager::Action_t action;
    int32_t actor;
    CHIP_ERROR err = CHIP_NO_ERROR;

    if (aEvent->Type == AppEvent::kEventType_Lock)
    {
        action = static_cast<LockManager::Action_t>(aEvent->LockEvent.Action);
        actor  = aEvent->LockEvent.Actor;
    }
    else if (aEvent->Type == AppEvent::kEventType_Button)
    {
        if (LockMgr().NextState() == true)
        {
            action = LockManager::LOCK_ACTION;
        }
        else
        {
            action = LockManager::UNLOCK_ACTION;
        }
        actor = AppEvent::kEventType_Button;
    }
    else
    {
        err = APP_ERROR_UNHANDLED_EVENT;
    }

    if (err == CHIP_NO_ERROR)
    {
        initiated = LockMgr().InitiateAction(actor, action);

        if (!initiated)
        {
            SILABS_LOG("Action is already in progress or active.");
        }
    }
}

//< WW2023: Add light code event handler
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
#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
    else if (aEvent->Type == AppEvent::kEventType_Button)
    {
        action = (LightMgr().IsLightOn()) ? LightingManager::OFF_ACTION : LightingManager::ON_ACTION;
        actor  = AppEvent::kEventType_Button;
    }
#endif
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
//>

#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
void AppTask::ButtonEventHandler(const sl_button_t * buttonHandle, uint8_t btnAction)
{
    if (buttonHandle == NULL)
    {
        return;
    }

    AppEvent button_event           = {};
    button_event.Type               = AppEvent::kEventType_Button;
    button_event.ButtonEvent.Action = btnAction;

    if (buttonHandle == APP_LOCK_SWITCH && btnAction == SL_SIMPLE_BUTTON_PRESSED)
    {
        button_event.Handler = LockActionEventHandler;
        sAppTask.PostEvent(&button_event);
    }
    else if (buttonHandle == APP_FUNCTION_BUTTON && btnAction == SL_SIMPLE_BUTTON_PRESSED)
    {
        //< WW2023: Add light button event; replaces QR code switch/factory reset functions
        /*
        button_event.Handler = BaseApplication::ButtonHandler;
        sAppTask.PostEvent(&button_event);
        */
        button_event.Handler = LightActionEventHandler;
        sAppTask.PostEvent(&button_event);
        //>
    }
}
#endif

void AppTask::ActionInitiated(LockManager::Action_t aAction, int32_t aActor)
{
    if (aAction == LockManager::UNLOCK_ACTION || aAction == LockManager::LOCK_ACTION)
    {
        bool locked = (aAction == LockManager::LOCK_ACTION);
        SILABS_LOG("%s Action has been initiated", (locked) ? "Lock" : "Unlock");
        sLockLED.Set(!locked);

#ifdef DISPLAY_ENABLED
        // WW2023: Add dual state support
        sAppTask.GetLCD().WriteDemoUI(locked, true);
#endif // DISPLAY_ENABLED
    }

    if (aActor == AppEvent::kEventType_Button)
    {
        sAppTask.mSyncClusterToButtonAction = true;
    }
}

void AppTask::ActionCompleted(LockManager::Action_t aAction)
{
    // if the action has been completed by the lock, update the lock trait.
    // Turn off the lock LED if in a LOCKED state OR
    // Turn on the lock LED if in an UNLOCKED state.
    if (aAction == LockManager::LOCK_ACTION)
    {
        SILABS_LOG("Lock Action has been completed")
    }
    else if (aAction == LockManager::UNLOCK_ACTION)
    {
        SILABS_LOG("Unlock Action has been completed")
    }

    if (sAppTask.mSyncClusterToButtonAction)
    {
        chip::DeviceLayer::PlatformMgr().ScheduleWork(UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));
        sAppTask.mSyncClusterToButtonAction = false;
    }
}

void AppTask::ActionRequest(int32_t aActor, LockManager::Action_t aAction)
{
    AppEvent event;
    event.Type             = AppEvent::kEventType_Lock;
    event.LockEvent.Actor  = aActor;
    event.LockEvent.Action = aAction;
    event.Handler          = LockActionEventHandler;
    PostEvent(&event);
}

void AppTask::UpdateClusterState(intptr_t context)
{
    bool unlocked        = LockMgr().NextState();
    DlLockState newState = unlocked ? DlLockState::kUnlocked : DlLockState::kLocked;

    OperationSourceEnum source = OperationSourceEnum::kUnspecified;

    // write the new lock value
    EmberAfStatus status =
        DoorLockServer::Instance().SetLockState(1, newState, source) ? EMBER_ZCL_STATUS_SUCCESS : EMBER_ZCL_STATUS_FAILURE;

    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        SILABS_LOG("ERR: updating lock state %x", status);
    }
}

//< WW2023: Add light ActionInitiated code
void AppTask::ActionInitiatedLight(LightingManager::Action_t aAction, int32_t aActor)
{
    // Action initiated, update the light led
    bool lightOn = aAction == LightingManager::ON_ACTION;
    SILABS_LOG("Turning light %s", (lightOn) ? "On" : "Off")

    sLightLED.Set(lightOn);

#ifdef DISPLAY_ENABLED
    // WW2023: Add dual state support
    sAppTask.GetLCD().WriteDemoUI(lightOn, false);
#endif
#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
    if (aActor == AppEvent::kEventType_Button)
    {
        sAppTask.mSyncClusterToButtonAction = true;
    }
#endif
}
//>

//< WW2023: Add light ActionCompleted code
void AppTask::ActionCompletedLight(LightingManager::Action_t aAction)
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
#ifdef SL_CATALOG_SIMPLE_BUTTON_PRESENT
    if (sAppTask.mSyncClusterToButtonAction)
    {
        // WW2023: Modify callback names (add Light)
        chip::DeviceLayer::PlatformMgr().ScheduleWork(UpdateClusterStateLight, reinterpret_cast<intptr_t>(nullptr));
        sAppTask.mSyncClusterToButtonAction = false;
    }
#endif
}
//>

//< WW2023: Add light post light action request code
void AppTask::PostLightActionRequest(int32_t aActor, LightingManager::Action_t aAction)
{
    AppEvent event;
    event.Type              = AppEvent::kEventType_Light;
    event.LightEvent.Actor  = aActor;
    event.LightEvent.Action = aAction;
    event.Handler           = LightActionEventHandler;
    PostEvent(&event);
}
//>

//< WW2023: Add light cluster update code
void AppTask::UpdateClusterStateLight(intptr_t context)
{
    uint8_t newValue = LightMgr().IsLightOn();

    // write the new on/off value
    // WW2023: Set endpoint to 2 to match ZAP configuration
    EmberAfStatus status = OnOffServer::Instance().setOnOffValue(2, newValue, false);

    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        SILABS_LOG("ERR: updating on/off %x", status);
    }
}
//>
