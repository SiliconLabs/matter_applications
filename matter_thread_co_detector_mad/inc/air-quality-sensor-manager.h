/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include \
  <app/clusters/concentration-measurement-server/concentration-measurement-server.h>

#pragma once

namespace chip {
  namespace app {
    namespace Clusters {
      class AirQualitySensorManager
      {
public:
        // Delete copy constructor and assignment operator.
        AirQualitySensorManager(const AirQualitySensorManager &) = delete;
        AirQualitySensorManager(const AirQualitySensorManager &&) = delete;
        AirQualitySensorManager & operator =
          (const AirQualitySensorManager &) = delete;

        static void InitInstance(EndpointId aEndpointId = 1)
        {
          if (mInstance == nullptr) {
            mInstance = new AirQualitySensorManager(aEndpointId);

            mInstance->Init();
          }
        }

        /**
         * @brief Get an Air Quality Manager object - this class acts as a
         *   singleton device manager for the air quality device
         * @param[in] aEndpointId    Endpoint that the air quality is on
         * @return mInstance    The AirQualitySensorManager instance, note
         *   this could be nullptr if InitInstance has not been
         * called
         */
        static AirQualitySensorManager * GetInstance()
        {
          return mInstance;
        }

        /**
         * @brief Handles changes in Carbon Dioxide concentration measurement.
         * @param[in] newValue The new air value to be applied.
         */
        void Init();

        /**
         * @brief Handles changes in Carbon Monoxide concentration measurement.
         * @param[in] newValue The new air value to be applied.
         */
        void OnCarbonMonoxideMeasurementChangeHandler(float newValue);

private:
        inline static AirQualitySensorManager *mInstance;
        EndpointId mEndpointId;
        ConcentrationMeasurement::Instance < true, true, true,
        true, true, true
        > mCarbonMonoxideConcentrationMeasurementInstance;

        /**
         * @brief Construct a new Air Quality Manager object - this class acts
         *   as asingleton device manager for the air quality device
         * @param[in] endpointId    Endpoint that the air quality device is on
         */
        AirQualitySensorManager(EndpointId aEndpointId) :
          mEndpointId(aEndpointId),
          mCarbonMonoxideConcentrationMeasurementInstance(mEndpointId,
                                                          CarbonMonoxideConcentrationMeasurement::Id,
                                                          ConcentrationMeasurement::MeasurementMediumEnum::kAir,
                                                          ConcentrationMeasurement::MeasurementUnitEnum::kPpm)
        {
        }
      };
    }     // namespace Clusters
  }   // namespace app
} // namespace chip
