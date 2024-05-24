#include "AccessorsExtension.h"
#include <app/util/af.h>
#include <app-common/zap-generated/attribute-type.h>


namespace chip {
namespace app {
namespace Clusters {

namespace WeatherStation {
namespace Attributes {

namespace Rainfall {

EmberAfStatus Get(chip::EndpointId endpoint, float * value)
{
    using Traits = NumericAttributeTraits<float>;
    Traits::StorageType temp;
    uint8_t * readable   = Traits::ToAttributeStoreRepresentation(temp);
    EmberAfStatus status = emberAfReadAttribute(endpoint, Clusters::WeatherStation::Id, Id, readable, sizeof(temp));
    VerifyOrReturnError(EMBER_ZCL_STATUS_SUCCESS == status, status);
    if (!Traits::CanRepresentValue(/* isNullable = */ false, temp))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    *value = Traits::StorageToWorking(temp);
    return status;
}
EmberAfStatus Set(chip::EndpointId endpoint, float value)
{
    using Traits = NumericAttributeTraits<float>;
    if (!Traits::CanRepresentValue(/* isNullable = */ false, value))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    Traits::StorageType storageValue;
    Traits::WorkingToStorage(value, storageValue);
    uint8_t * writable = Traits::ToAttributeStoreRepresentation(storageValue);
    return emberAfWriteAttribute(endpoint, Clusters::WeatherStation::Id, Id, writable, ZCL_SINGLE_ATTRIBUTE_TYPE);
}

} // namespace Rainfall
namespace WindDirection {

EmberAfStatus Get(chip::EndpointId endpoint, uint32_t * value)
{
    using Traits = NumericAttributeTraits<uint32_t>;
    Traits::StorageType temp;
    uint8_t * readable   = Traits::ToAttributeStoreRepresentation(temp);
    EmberAfStatus status = emberAfReadAttribute(endpoint, Clusters::WeatherStation::Id, Id, readable, sizeof(temp));
    VerifyOrReturnError(EMBER_ZCL_STATUS_SUCCESS == status, status);
    if (!Traits::CanRepresentValue(/* isNullable = */ false, temp))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    *value = Traits::StorageToWorking(temp);
    return status;
}
EmberAfStatus Set(chip::EndpointId endpoint, uint32_t value)
{
    using Traits = NumericAttributeTraits<uint32_t>;
    if (!Traits::CanRepresentValue(/* isNullable = */ false, value))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    Traits::StorageType storageValue;
    Traits::WorkingToStorage(value, storageValue);
    uint8_t * writable = Traits::ToAttributeStoreRepresentation(storageValue);
    return emberAfWriteAttribute(endpoint, Clusters::WeatherStation::Id, Id, writable, ZCL_INT32U_ATTRIBUTE_TYPE);
}

} // namespace WindDirection

namespace WindSpeed {

EmberAfStatus Get(chip::EndpointId endpoint, float * value)
{
    using Traits = NumericAttributeTraits<float>;
    Traits::StorageType temp;
    uint8_t * readable   = Traits::ToAttributeStoreRepresentation(temp);
    EmberAfStatus status = emberAfReadAttribute(endpoint, Clusters::WeatherStation::Id, Id, readable, sizeof(temp));
    VerifyOrReturnError(EMBER_ZCL_STATUS_SUCCESS == status, status);
    if (!Traits::CanRepresentValue(/* isNullable = */ false, temp))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    *value = Traits::StorageToWorking(temp);
    return status;
}
EmberAfStatus Set(chip::EndpointId endpoint, float value)
{
    using Traits = NumericAttributeTraits<float>;
    if (!Traits::CanRepresentValue(/* isNullable = */ false, value))
    {
        return EMBER_ZCL_STATUS_CONSTRAINT_ERROR;
    }
    Traits::StorageType storageValue;
    Traits::WorkingToStorage(value, storageValue);
    uint8_t * writable = Traits::ToAttributeStoreRepresentation(storageValue);
    return emberAfWriteAttribute(endpoint, Clusters::WeatherStation::Id, Id, writable, ZCL_SINGLE_ATTRIBUTE_TYPE);
}

} // namespace WindSpeed
} // namespace Attributes
} // namespace WeatherStation

} // namespace Clusters
} // namespace app
} // namespace chip
