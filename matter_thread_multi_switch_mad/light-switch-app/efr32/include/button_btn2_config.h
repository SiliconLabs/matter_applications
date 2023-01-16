/*****************************************************************************
 * @file
 * @brief Button 2 Driver User Config
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef BUTTON_BTN2_CONFIG_H
#define BUTTON_BTN2_CONFIG_H

#include "em_gpio.h"
#include "sl_simple_button.h"

#define SL_SIMPLE_BUTTON_BTN2_MODE       SL_SIMPLE_BUTTON_MODE_INTERRUPT
#define SL_SIMPLE_BUTTON_BTN2_PORT       gpioPortD
#define SL_SIMPLE_BUTTON_BTN2_PIN        8

#endif // BUTTON_BTN2_CONFIG_H
