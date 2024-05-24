
#include <app/util/af-types.h>
#include <app/util/basic-types.h>
#include <app/AttributeAccessInterface.h>
#include <app/CommandHandlerInterface.h>

using namespace chip;
using namespace app;

CHIP_ERROR UpdateRainfall(EndpointId, float);
CHIP_ERROR UpdateWindSpeed(EndpointId, float);
CHIP_ERROR UpdateWindDirection(EndpointId, uint32_t);
CHIP_ERROR UpdateTemperature(EndpointId, int16_t);
CHIP_ERROR UpdateHumidity(EndpointId, uint16_t);
