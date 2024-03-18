// Specific includes to this cluster
#include <app/clusters/led-strip-server/led-strip-server.h>

// Common includes for the led-strip cluster
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>
#include <app/util/af.h>
#include <app/util/config.h>
#include <app/util/error-mapping.h>
#include <app/util/util.h>

#include <app/reporting/reporting.h>
#include <lib/core/Optional.h>
#include <platform/CHIPDeviceConfig.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/PlatformManager.h>

/**********************************************************
 * Private functions to the cluster
 *********************************************************/

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::LedStrip;

LedStripServer LedStripServer::instance;

LedStripServer & LedStripServer::Instance()
{
    return instance;
}

EmberAfStatus LedStripServer::getRedColor(chip::EndpointId endpoint, uint8_t * redColor)
{
    EmberAfStatus status = Attributes::RedColor::Get(endpoint, redColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading RedColor %x", status);
    }

    ChipLogProgress(Zcl, "RedValue ep%d value: %d", endpoint, *redColor);
    return status;
}

EmberAfStatus LedStripServer::getGreenColor(chip::EndpointId endpoint, uint8_t * greenColor)
{
    EmberAfStatus status = Attributes::GreenColor::Get(endpoint, greenColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading GreenColor %x", status);
    }

    ChipLogProgress(Zcl, "GreenValue ep%d value: %d", endpoint, *greenColor);
    return status;
}

EmberAfStatus LedStripServer::getBlueColor(chip::EndpointId endpoint, uint8_t * blueColor)
{
    EmberAfStatus status = Attributes::BlueColor::Get(endpoint, blueColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading BlueColor %x", status);
    }

    ChipLogProgress(Zcl, "GreenValue ep%d value: %d", endpoint, *blueColor);
    return status;
}

EmberAfStatus LedStripServer::getStateStrip(chip::EndpointId endpoint, uint8_t * stateStrip)
{
    EmberAfStatus status = Attributes::StateStrip::Get(endpoint, stateStrip);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading stateStrip %x", status);
    }

    ChipLogProgress(Zcl, "stateStrip ep%d value: %d", endpoint, *stateStrip);
    return status;
}
    
EmberAfStatus LedStripServer::getOnOffStrip(chip::EndpointId endpoint, bool * onOffValue)
{
    EmberAfStatus status = Attributes::OnOffStrip::Get(endpoint, onOffValue);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading onOffValue %x", status);
    }

    ChipLogProgress(Zcl, "OnOffValue ep%d value: %d", endpoint, *onOffValue);
    return status;
}

EmberAfStatus LedStripServer::setRedColor(chip::EndpointId endpoint, uint8_t redColor)
{
    EmberAfStatus status = Attributes::RedColor::Set(endpoint, redColor);

    return status;
}

EmberAfStatus LedStripServer::setGreenColor(chip::EndpointId endpoint, uint8_t greenColor)
{
    EmberAfStatus status;
    status = Attributes::GreenColor::Set(endpoint, greenColor);

    return status;
}

EmberAfStatus LedStripServer::setBlueColor(chip::EndpointId endpoint, uint8_t blueColor)
{
    EmberAfStatus status;
    status = Attributes::BlueColor::Set(endpoint,blueColor);

    return status;
}

EmberAfStatus LedStripServer::setColor(chip::EndpointId endpoint, uint8_t redColor, uint8_t greenColor, uint8_t blueColor)
{
    EmberAfStatus status = setRedColor(endpoint, redColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS) {
        ChipLogProgress(Zcl, "LedStrip : setColor red failed endpoint %x", endpoint);
        return status;
    }

    status = setGreenColor(endpoint, greenColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS) {
        ChipLogProgress(Zcl, "LedStrip : setColor green failed endpoint %x", endpoint);
        return status;
    }

    status = setBlueColor(endpoint, blueColor);
    if (status != EMBER_ZCL_STATUS_SUCCESS) {
        ChipLogProgress(Zcl, "LedStrip : setColor blue failed endpoint %x", endpoint);
        return status;
    }
    
    return EMBER_ZCL_STATUS_SUCCESS;
}

/*
 * Function called to set the OnOff value of the endpoint.
 */
EmberAfStatus LedStripServer::SetOnOffValue(chip::EndpointId endpoint, chip::CommandId command, bool onOffState)
{
    EmberAfStatus status;
    bool currentValue;
    
    status = Attributes::OnOffStrip::Get(endpoint, &currentValue);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading ledStrip onoff %x", status);
        return status;
    }

    // Check if the value is already the correct one
    if (currentValue == onOffState) 
    {
        ChipLogProgress(Zcl, "Endpoint %x LedStrip onoff already set to this value", endpoint);
        return EMBER_ZCL_STATUS_SUCCESS;
    }

    status = Attributes::OnOffStrip::Set(endpoint, onOffState);
    if (status != EMBER_ZCL_STATUS_SUCCESS) 
    {
        ChipLogProgress(Zcl, "Err: Writing onOffStrip %x", status);
        return status;
    }

    return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus LedStripServer::SetStateValue(chip::EndpointId endpoint, chip::CommandId command, uint8_t stateValue)
{
    EmberAfStatus status;
    uint8_t currentValue;
    
    status = Attributes::StateStrip::Get(endpoint, &currentValue);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        ChipLogProgress(Zcl, "ERR: reading ledStrip StateStrip %x", status);
        return status;
    }

    // Check if the value is already the correct one
    if (currentValue == stateValue) 
    {
        ChipLogProgress(Zcl, "Endpoint %x LedStrip StateStrip already set to this value", endpoint);
        return EMBER_ZCL_STATUS_SUCCESS;
    }

    status = Attributes::StateStrip::Set(endpoint, stateValue);
    if (status != EMBER_ZCL_STATUS_SUCCESS) 
    {
        ChipLogProgress(Zcl, "Err: Writing onOffStrip %x", status);
        return status;
    }

    return EMBER_ZCL_STATUS_SUCCESS;
}

bool LedStripServer::SetStateCommand(CommandHandler * commandObj, 
                                     const ConcreteCommandPath & commandPath,
                                     const Commands::SetState::DecodableType & commandData)
{
    EmberAfStatus status = SetStateValue(commandPath.mEndpointId, Commands::SetState::Id, commandData.stateValue);

    commandObj->AddStatus(commandPath, app::ToInteractionModelStatus(status));
    return true;
}

bool LedStripServer::SetColorCommand(CommandHandler * commandObj, 
                                     const ConcreteCommandPath & commandPath,
                                     const Commands::SetColor::DecodableType & commandData)
{
    EmberAfStatus status = setColor(commandPath.mEndpointId, commandData.redColor, commandData.greenColor, commandData.blueColor);

    commandObj->AddStatus(commandPath, app::ToInteractionModelStatus(status));
    return true;
}

bool LedStripServer::SetOnOffCommand(CommandHandler * commandObj, 
                                     const ConcreteCommandPath & commandPath,
                                     const Commands::OnOffStrip::DecodableType & commandData)
{
    EmberAfStatus status = SetOnOffValue(commandPath.mEndpointId, Commands::OnOffStrip::Id, commandData.onOffValue);

    commandObj->AddStatus(commandPath, app::ToInteractionModelStatus(status));
    return true;
}

void initLedStripServer(EndpointId endpoint) {}

/**********************************************************
 * Callbacks
 *********************************************************/

bool emberAfLedStripClusterSetColorCallback(CommandHandler * commandObj, 
                                            const ConcreteCommandPath & commandPath,
                                            const Commands::SetColor::DecodableType & commandData)
{
    return LedStripServer::Instance().SetColorCommand(commandObj, commandPath, commandData);
}

bool emberAfLedStripClusterSetStateCallback(CommandHandler * commandObj, 
                                            const ConcreteCommandPath & commandPath,
                                            const Commands::SetState::DecodableType & commandData)
{
    return LedStripServer::Instance().SetStateCommand(commandObj, commandPath, commandData);
}

bool emberAfLedStripClusterOnOffStripCallback(CommandHandler * commandObj, 
                                              const ConcreteCommandPath & commandPath,
                                              const Commands::OnOffStrip::DecodableType & commandData)
{
    return LedStripServer::Instance().SetOnOffCommand(commandObj, commandPath, commandData);
}

void emberAfLedStripClusterServerInitCallback(EndpointId endpoint) 
{
    // Initialization function
    LedStripServer::Instance().initLedStripServer(endpoint);
}

void MatterLedStripPluginServerInitCallback() {}