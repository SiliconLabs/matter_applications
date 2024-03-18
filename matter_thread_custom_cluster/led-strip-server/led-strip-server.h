#pragma once

#include <stdint.h>

#include <app-common/zap-generated/cluster-enums.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app/util/basic-types.h>
#include <app/util/af-types.h>
#include <platform/CHIPDeviceConfig.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>

/**********************************************************
 * Class / Private functions
 *********************************************************/

class LedStripServer 
{
public:
    /**********************************************************
     * Functions Definitions
     *********************************************************/
    
    static LedStripServer & Instance();

    EmberAfStatus getRedColor(chip::EndpointId endpoint, uint8_t * redColor);
    EmberAfStatus getGreenColor(chip::EndpointId endpoint, uint8_t * greenColor);
    EmberAfStatus getBlueColor(chip::EndpointId endpoint, uint8_t * blueColor);
    EmberAfStatus getStateStrip(chip::EndpointId endpoint, uint8_t * stateStrip);
    EmberAfStatus getOnOffStrip(chip::EndpointId endpoint, bool * onOffValue);
    
    EmberAfStatus setRedColor(chip::EndpointId endpoint, uint8_t redColor);
    EmberAfStatus setGreenColor(chip::EndpointId endpoint, uint8_t greenColor);
    EmberAfStatus setBlueColor(chip::EndpointId endpoint, uint8_t blueColor);

    EmberAfStatus setColor(chip::EndpointId, uint8_t redColor, uint8_t greenColor, uint8_t blueColor);
    EmberAfStatus SetOnOffValue(chip::EndpointId endpoint, chip::CommandId command, bool onOffState);
    EmberAfStatus SetStateValue(chip::EndpointId endpoint, chip::CommandId command, uint8_t stateValue);

    // List of commands.
    bool SetStateCommand(chip::app::CommandHandler * commandObj, 
                         const chip::app::ConcreteCommandPath & commandPath,
                         const chip::app::Clusters::LedStrip::Commands::SetState::DecodableType & commandData);
    bool SetColorCommand(chip::app::CommandHandler * commandObj, 
                         const chip::app::ConcreteCommandPath & commandPath,
                         const chip::app::Clusters::LedStrip::Commands::SetColor::DecodableType & commandData);
    bool SetOnOffCommand(chip::app::CommandHandler * commandObj, 
                         const chip::app::ConcreteCommandPath & commandPath,
                         const chip::app::Clusters::LedStrip::Commands::OnOffStrip::DecodableType & commandData);                     
    
    void initLedStripServer(chip::EndpointId endpoint);
private:
    /**********************************************************
     * Functions Definitions
     *********************************************************/
    static LedStripServer instance;
};

/**********************************************************
 * Callbacks/Global
 *********************************************************/

bool emberAfLedStripClusterSetColorCallback(chip::app::CommandHandler * commandObj, 
                                              const chip::app::ConcreteCommandPath & commandPath,
                                              const chip::app::Clusters::LedStrip::Commands::SetColor::DecodableType & commandData);

bool emberAfLedStripClusterSetStateCallback(chip::app::CommandHandler* commandObj, 
                                              const chip::app::ConcreteCommandPath & commandPath,
                                              const chip::app::Clusters::LedStrip::Commands::SetState::DecodableType & commandData);

bool emberAfLedStripClusterOnOffStripCallback(chip::app::CommandHandler * commandObj, 
                                                const chip::app::ConcreteCommandPath & commandPath,
                                                const chip::app::Clusters::LedStrip::Commands::OnOffStrip::DecodableType & commandData);

void emberAfLedStripClusterServerInitCallback(chip::EndpointId endpoint);

void MatterLedStripPluginServerInitCallback();