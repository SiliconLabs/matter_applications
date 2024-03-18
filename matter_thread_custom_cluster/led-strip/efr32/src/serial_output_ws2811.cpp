/***************************************************************************//**
 * @file serial_output_ws2811.c
 * @brief Serial output for ws2811 source code
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

/**********************************************************
 * Includes / Variables
 *********************************************************/

#include  <serial_output_ws2811.h>

#include "sl_power_manager.h"
#include "em_core.h"

#define HIGH_CODE_DUTYCYCLE 48u
#define LOW_CODE_DUTYCYCLE 20u

#define NUMBER_OF_COLOR_BITS (NUMBER_LEDS * 3 * 8)

/* For a 0  : It starts with a High and ends with Low.
 *
 * Frequency of the protocol : 400 kHz.
 *
 * 0.5 us of high state and 2 us of low state
 *      Duty cycle : 20 %
 *
 * For a 1  : Starts with a High and ends with a Low
 * 1.2 us of high state and 1.3 us of low state
 *      Duty cycle : 48 %
 * Number of bits for USART : 6
 * Frequency : 6 * 400 kHz.
 *
 * So for 0 : 1 0 0 0 0 0
 * It means : 0.416 us for 1 => 16.7 % dutycycle
 *
 * So for 1 : 1 1 1 0 0 0
 * It means : 1.25  us for 1 => 50 % dutycycle
 *
 * For a reset signal : at least 50 us
 * Each coded is 2.5 us (for 400 kHz)
 * That means 6 * 20 = 120 => 15 bytes
 *
 */

/* Define for the USART */
#define USART_NUMBER_BITS_CODE 6
#define USART_NUMBER_RESET_BYTES 20
#define USART_NUMBER_COLOR_BITS NUMBER_OF_COLOR_BITS * USART_NUMBER_BITS_CODE
#define USART_BUFFER_BYTES_SIZE ((USART_NUMBER_COLOR_BITS / 8) + USART_NUMBER_RESET_BYTES)

#define USART_FREQUENCY STRIP_PWM_FREQ * USART_NUMBER_BITS_CODE

#define USART_TX_PORT gpioPortA
#define USART_TX_PIN 8

static uint8_t USART_TX_buffer[USART_BUFFER_BYTES_SIZE] = {0};

static unsigned int ldmaTxCh = 1;

#define FIRST_BYTE_DEFAULT_VALUE 0x82; //  1xx0 001x  Byte default
#define SECOND_BYTE_DEFAULT_VALUE 0x08; // x000 1xx0  Byte default
#define THIRD_BYTE_DEFAULT_VALUE 0x20; //  001x x000  Byte default
#define FOURTH_BYTE_DEFAULT_VALUE 0x82; // 1xx0 001x  Byte default
#define FIFTH_BYTE_DEFAULT_VALUE 0x08; //  x000 1xx0  Byte default
#define SIXTH_BYTE_DEFAULT_VALUE 0x20; //  001x x000  Byte default
// 4 bytes for one color, 2 bits by Byte.

/**********************************************************
 * Functions
 *********************************************************/

void initStripGPIOs(void)
{
  // Enable GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);

  // Init GPIOs
  GPIO_PinModeSet(USART_TX_PORT, USART_TX_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 0);
  GPIO_SlewrateSet(USART_TX_PORT, 6, 6);
}

void initStripUSART(void)
{
  // Start with default config, then modify as necessary
  USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
  config.master       = true;            // Master mode
  config.baudrate     = USART_FREQUENCY;
  config.clockMode    = usartClockMode0; // Clock idle low, sample on rising/first edge
  config.msbf         = true;            // Send MSB first
  config.enable       = usartDisable;    // Keep USART disabled until it's all set up

  USART_InitSync(USART0, &config);

  // Route USART0 TX and RX to the board controller TX and RX pins
  GPIO->USARTROUTE[0].TXROUTE = (USART_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
            | (USART_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

  GPIO->USARTROUTE[0].CLKROUTE = (gpioPortA << _GPIO_USART_CLKROUTE_PORT_SHIFT)
            | (5 << _GPIO_USART_CLKROUTE_PIN_SHIFT);

  // Enable RX and TX signals now that they have been routed
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN | GPIO_USART_ROUTEEN_CLKPEN;

  // Enable USART
  USART_Enable(USART0, usartEnableTx);
  USART_Tx(USART0, 0);
}

bool LDMA_Callback(unsigned int channel,
                   unsigned int sequenceNo,
                   void *userParam)
{
  return false;
}

/*
 * Initialize the LDMA module
 *
 */
void initLDMA(void)
{
  CMU_ClockEnable(cmuClock_LDMA, true);

  // Project on Matter requires to use DMADRV
  DMADRV_Init();
}

/**
 * @brief
 * Change the power management requirement once the DMA Transmission is done
 */
bool transmitDMADone(unsigned int channel, unsigned int sequenceNo, void *userParam)
{
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);

  return true;
}

/*
 * @brief
 * Populate the Timer Buffer with calculated duty cycle time values
 */
void populate_usart_buffer(uint8_t *input_color_buffer)
{
  uint32_t usart_buffer_index = 0; // First byte are 0
  uint8_t *current_byte = input_color_buffer;

  uint8_t bits[8] = {0};
  // Bit sequence is : 1x00 1x00
  while (usart_buffer_index < (USART_BUFFER_BYTES_SIZE - USART_NUMBER_RESET_BYTES)) {
      //  1xx0 001x  Byte default
      bits[7] = ((*current_byte & 0x80) >> 1) | ((*current_byte & 0x80) >> 2);
      bits[6] = (*current_byte & 0x40) >> 6;
      USART_TX_buffer[usart_buffer_index] = bits[7] | bits[6] | FIRST_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      // x000 1xx0  Byte default
      bits[6] = (*current_byte & 0x40) << 1;
      bits[5] = ((*current_byte & 0x20) >> 3) | ((*current_byte & 0x20) >> 4);
      USART_TX_buffer[usart_buffer_index] = bits[6] | bits[5] | SECOND_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      //  001x x000  Byte default
      bits[4] = (*current_byte & 0x10) | ((*current_byte & 0x10) >> 1);
      USART_TX_buffer[usart_buffer_index] = bits[4] | THIRD_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      // 1xx0 001x  Byte default
      bits[3] = ((*current_byte & 0x08) << 3) | ((*current_byte & 0x08) << 2);
      bits[2] = (*current_byte & 0x04) >> 2;
      USART_TX_buffer[usart_buffer_index] = bits[3] | bits[2] | FOURTH_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      //  x000 1xx0  Byte default
      bits[2] = (*current_byte & 0x04) << 5;
      bits[1] = ((*current_byte & 0x02) << 1) | (*current_byte & 0x02);
      USART_TX_buffer[usart_buffer_index] = bits[2] | bits[1] | FIFTH_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      //  001x x000  Byte default
      bits[0] = ((*current_byte & 0x01) << 3) | ((*current_byte & 0x01) << 4);
      USART_TX_buffer[usart_buffer_index] = bits[0] | SIXTH_BYTE_DEFAULT_VALUE;
      usart_buffer_index++;

      current_byte++;
  }

  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

  DMADRV_AllocateChannel(&ldmaTxCh,0);  
  DMADRV_MemoryPeripheral(ldmaTxCh,
                          dmadrvPeripheralSignal_USART0_TXBL,
                          (void*)&(USART0->TXDATA),
                          USART_TX_buffer,
                          true,
                          USART_BUFFER_BYTES_SIZE,
                          ldmaCtrlSizeByte,
                          transmitDMADone,
                          0);
}
