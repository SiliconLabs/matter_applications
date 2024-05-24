
#include <app-common/zap-generated/cluster-objects.h>
#include <app/data-model/Nullable.h>
#include <app/util/af-types.h>
#include <lib/support/Span.h>

namespace chip {
  namespace app {
    namespace Clusters {
      namespace WeatherStation {
        namespace Attributes {
          namespace Rainfall {
            static constexpr AttributeId Id = 0x0002;
            EmberAfStatus Get(chip::EndpointId endpoint, float *value);  // float
            EmberAfStatus Set(chip::EndpointId endpoint, float value);
          } // namespace Rainfall
          namespace WindDirection {
            static constexpr AttributeId Id = 0x0001;
            EmberAfStatus Get(chip::EndpointId endpoint, uint32_t *value);  // uint32_t
            EmberAfStatus Set(chip::EndpointId endpoint, uint32_t value);
          } // namespace WindDirection
          namespace WindSpeed {
            static constexpr AttributeId Id = 0x0000;
            EmberAfStatus Get(chip::EndpointId endpoint, float *value);  // float
            EmberAfStatus Set(chip::EndpointId endpoint, float value);
          } // namespace WindSpeed
        } // namespace Attributes
      } // namespace WeatherStation
    } // namespace Clusters
  } // namespace app
} // namespace chip
