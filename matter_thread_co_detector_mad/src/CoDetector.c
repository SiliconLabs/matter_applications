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

#include "CoDetector.h"

/***************************************************************************//**
 * Initialize the CO detector.
 ******************************************************************************/
sl_status_t CoDetector_init(void)
{
  return mikroe_mq7_init(IADC0);
}

/***************************************************************************//**
 * Read the CO detector value.
 ******************************************************************************/
uint16_t CoDetector_read(void)
{
  uint16_t data_out = 0;

  mikroe_mq7_read_an_pin_value(&data_out);

  return data_out;
}
