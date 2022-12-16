/***************************************************************************//**
 * @file
 * @brief Button 2 Driver Instance
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

#include "button_btn2_instance.h"
#include "sl_simple_button.h"
#include "button_btn2_config.h"

sl_simple_button_context_t simple_btn2_context = {
  .state = 0,
  .history = 0,
  .port = SL_SIMPLE_BUTTON_BTN2_PORT,
  .pin = SL_SIMPLE_BUTTON_BTN2_PIN,
  .mode = SL_SIMPLE_BUTTON_BTN2_MODE,
};

const sl_button_t sl_button_btn2 = {
  .context = &simple_btn2_context,
  .init = sl_simple_button_init,
  .get_state = sl_simple_button_get_state,
  .poll = sl_simple_button_poll_step,
  .enable = sl_simple_button_enable,
  .disable = sl_simple_button_disable,
};

void button_btn2_init_instance()
{
  sl_button_init(&sl_button_btn2);
}
