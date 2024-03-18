/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

/**
 * @file
 *   This file implements the handler for data model messages.
 */

#include "AppConfig.h"
#include "AppTask.h"

#include "led_strip_driver.h"
#include <app/clusters/led-strip-server/led-strip-server.h>

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <lib/support/logging/CHIPLogging.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    ClusterId clusterId     = attributePath.mClusterId;
    AttributeId attributeId = attributePath.mAttributeId;
    //EndpointId endpoint     = attributePath.mEndpointId;
    MyLedStrip::action_t action_type = MyLedStrip::IGNORE_ACTION;

    bool action_initiated = true;

    ChipLogProgress(Zcl, "Cluster callback: " ChipLogFormatMEI, ChipLogValueMEI(clusterId));

    if (clusterId == LedStrip::Id)
    {
        SILABS_LOG("Entering Post Attribute ledStripServer::Id");
        if (attributeId == LedStrip::Attributes::RedColor::Id) {
            SILABS_LOG("Action type is MOVE_TO_RED");
            action_type = MyLedStrip::MOVE_TO_RED;
        } else if (attributeId == LedStrip::Attributes::GreenColor::Id) {
            SILABS_LOG("Action type is MOVE_TO_GREEN");
            action_type = MyLedStrip::MOVE_TO_GREEN;
        } else if (attributeId == LedStrip::Attributes::BlueColor::Id)  {
            SILABS_LOG("Action type is MOVE_TO_BLUE");
            action_type = MyLedStrip::MOVE_TO_BLUE;
        } else if (attributeId == LedStrip::Attributes::OnOffStrip::Id) {
            SILABS_LOG("Action type is MOVE_TO_ONOFF");
            action_type = MyLedStrip::MOVE_TO_ONOFF;
        } else if (attributeId == LedStrip::Attributes::StateStrip::Id) {
            SILABS_LOG("Action type is MOVE_TO_STATE");
            action_type = MyLedStrip::MOVE_TO_STATE;
        } else {
            action_initiated = false;
        }

    } else {
        action_initiated = false;
    }
    
    // Calls the dispatcher
    if (action_initiated == true) {
        AppTask::GetAppTask().PostStripControlActionRequest(AppEvent::kEventType_Strip, action_type, *value);
    }
    
}

/** @brief OnOff Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 * TODO Issue #3841
 * emberAfOnOffClusterInitCallback happens before the stack initialize the cluster
 * attributes to the default value.
 * The logic here expects something similar to the deprecated Plugins callback
 * emberAfPluginOnOffClusterServerPostInitCallback.
 *
 */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
    // TODO: implement any additional Cluster Server init actions
}
