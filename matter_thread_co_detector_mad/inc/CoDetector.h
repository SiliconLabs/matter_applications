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

#ifndef INCLUDE_CODETECTOR_H_
#define INCLUDE_CODETECTOR_H_

#include "sl_status.h"
#include "em_iadc.h"
#include "mikroe_mq7.h"

/***************************************************************************//**
 * Initialize the CO detector.
 ******************************************************************************/
sl_status_t CoDetector_init(void);

/***************************************************************************//**
 * Read the CO detector value.
 ******************************************************************************/
uint16_t CoDetector_read(void);

#endif /* INCLUDE_CODETECTOR_H_ */
