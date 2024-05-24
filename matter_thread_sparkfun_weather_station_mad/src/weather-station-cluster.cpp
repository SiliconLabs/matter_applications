#include "weather-station-cluster.h"
#include <app/ConcreteAttributePath.h>
#include <app/AttributeAccessInterface.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <platform/silabs/platformAbstraction/SilabsPlatform.h>

#include "AccessorsExtension.h"

using namespace chip;
using namespace app;
using namespace chip::app::Clusters::WeatherStation;
using namespace ::chip::DeviceLayer::Silabs;


void MatterWeatherStationPluginServerInitCallback()
{

}

CHIP_ERROR UpdateRainfall(EndpointId endpoint, float newValue)
{
  EmberAfStatus status;
  status = Attributes::Rainfall::Set(endpoint, newValue);
  if (status == EMBER_SUCCESS)
    return CHIP_NO_ERROR;
  else
    return CHIP_ERROR_NOT_FOUND;
}

CHIP_ERROR UpdateWindSpeed(EndpointId endpoint, float newValue)
{
  EmberAfStatus status;
  status = Attributes::WindSpeed::Set(endpoint, newValue);
  if (status == EMBER_SUCCESS)
    return CHIP_NO_ERROR;
  else
    return CHIP_ERROR_NOT_FOUND;
}

CHIP_ERROR UpdateWindDirection(EndpointId endpoint, uint32_t newValue)
{
  EmberAfStatus status;
  status = Attributes::WindDirection::Set(endpoint, newValue);
  if (status == EMBER_SUCCESS)
    return CHIP_NO_ERROR;
  else
    return CHIP_ERROR_NOT_FOUND;
}


CHIP_ERROR UpdateTemperature(EndpointId endpoint, int16_t newValue)
{
  EmberAfStatus status;
  status = Clusters::TemperatureMeasurement::Attributes::MeasuredValue::Set(endpoint, newValue);
  if (status == EMBER_SUCCESS)
    return CHIP_NO_ERROR;
  else
    return CHIP_ERROR_NOT_FOUND;
}

CHIP_ERROR UpdateHumidity(EndpointId endpoint, uint16_t newValue)
{
  EmberAfStatus status;
  status = Clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::Set(endpoint,newValue);
  if (status == EMBER_SUCCESS)
    return CHIP_NO_ERROR;
  else
    return CHIP_ERROR_NOT_FOUND;
}
