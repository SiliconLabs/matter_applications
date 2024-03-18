/***************************************************************************//**
 * @file serial_output_ws2811.h
 * @brief Serial output for ws2811 Header File
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SERIAL_OUTPUT_WS2811_H_
#define SERIAL_OUTPUT_WS2811_H_

#include "em_emu.h"
#include "em_cmu.h"
#include "em_ldma.h"
#include "stdbool.h"
#include "em_usart.h"
#include "dmadrv.h"

#define DATA_INPUT_PIN 0
#define DATA_INPUT_PORT 0

#define LENGTH_TIME_BIT 2500u // in ns
#define STRIP_PWM_FREQ 400000u
#define NUMBER_LEDS 50u // Define size of the buffer and so forth

void populate_usart_buffer(uint8_t* input_color_buffer);
void initStripGPIOs(void);
void initStripUSART(void);
void initLedGpios(void);
void initLDMA(void);

#endif /* SERIAL_OUTPUT_WS2811_H_ */
