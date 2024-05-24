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

#include <air-quality-sensor-manager.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::DataModel;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::ConcentrationMeasurement;
using namespace chip::app::Clusters::AirQuality;

void AirQualitySensorManager::Init()
{
  // CO
  mCarbonMonoxideConcentrationMeasurementInstance.Init();
  mCarbonMonoxideConcentrationMeasurementInstance.SetMinMeasuredValue(MakeNullable(
                                                                        0.0f));
  mCarbonMonoxideConcentrationMeasurementInstance.SetMaxMeasuredValue(MakeNullable(
                                                                        1000.0f));
  mCarbonMonoxideConcentrationMeasurementInstance.SetMeasuredValue(MakeNullable(
                                                                     2.0f));
  mCarbonMonoxideConcentrationMeasurementInstance.SetPeakMeasuredValue(MakeNullable(
                                                                         1.0f));
  mCarbonMonoxideConcentrationMeasurementInstance.SetPeakMeasuredValueWindow(320);
  mCarbonMonoxideConcentrationMeasurementInstance.SetAverageMeasuredValue(MakeNullable(
                                                                            1.0f));
  mCarbonMonoxideConcentrationMeasurementInstance.SetAverageMeasuredValueWindow(
    320);
  mCarbonMonoxideConcentrationMeasurementInstance.SetUncertainty(0.0f);
  mCarbonMonoxideConcentrationMeasurementInstance.SetLevelValue(
    LevelValueEnum::kLow);
}

void AirQualitySensorManager::OnCarbonMonoxideMeasurementChangeHandler(
  float newValue)
{
  mCarbonMonoxideConcentrationMeasurementInstance.SetMeasuredValue(MakeNullable(
                                                                     newValue));
  ChipLogDetail(NotSpecified, "Updated Carbon Monoxide value: %f", newValue);
}
