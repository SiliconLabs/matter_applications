/***************************************************************************//**
 * @file led_strip_driver.c
 * @brief Led Strip source code
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

#include "led_strip_driver.h"
#include "serial_output_ws2811.h"
#include "AppConfig.h"

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/PlatformManager.h>
#include "AppTask.h"
#include <em_usart.h>
#include <app/clusters/led-strip-server/led-strip-server.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;

#include <stdlib.h>
#include "stdint.h"

// LUT for gamma correction
const uint8_t dutyCycle_LUT[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/* 0->255 : R,G,B values are stored in this array
 * Careful : It's [LED 0] G, R, B | [LED 1] G, R, B
 */
static uint8_t led_strip_dutycycle[NUMBER_COLORS] = {0};

/**********************************************************
 * Functions
 *********************************************************/

/*  Public Functions  */
void MyLedStrip::getRGBColor(uint8_t* r, uint8_t* g, uint8_t* b)
{
  *r = getRedColor();
  *g = getGreenColor();
  *b = getBlueColor();
}

uint8_t MyLedStrip::getRedColor(void)
{
  return this->redValue;
}

uint8_t MyLedStrip::getGreenColor(void)
{
  return this->greenValue;
}

uint8_t MyLedStrip::getBlueColor(void)
{
  return this->blueValue;
}

uint8_t MyLedStrip::getStateStrip(void)
{
  return this->stripState;
}

bool MyLedStrip::getOnOffStrip(void)
{
  return this->onOffState;
}

bool MyLedStrip::setRedColor(uint8_t r)
{
  this->redValue = r;
  return true;
}

bool MyLedStrip::setGreenColor(uint8_t g)
{
  this->greenValue = g;
  return true;
}

bool MyLedStrip::setBlueColor(uint8_t b)
{
  this->blueValue = b;
  this->setColor(this->redValue, this->greenValue, this-> blueValue, 0, NUMBER_COLORS);
  return true;
}

bool MyLedStrip::setStateStrip(uint8_t state)
{
  this->stripState = state;
  return true;
}

bool MyLedStrip::setOnOffStrip(bool onOffValue)
{
  this->onOffState = onOffValue;
  if (onOffValue == true) {
    this->strip_turnOn();
  } else {
    this->strip_turnOff();
  }
  return true;
}

/*  Private Functions */
void MyLedStrip::resetBuffer(void)
{
  for(uint32_t index = 0; index < NUMBER_COLORS; index++)
  {
      led_strip_dutycycle[index] = 0;
  }
  populate_usart_buffer(led_strip_dutycycle);
}

/*
 * Function to set ledColor from one led to another
 */
void MyLedStrip::setColor(uint8_t r, uint8_t g, uint8_t b, uint32_t start, uint32_t end)
{
  // Check start and end does not touch the reset ?
  for(uint32_t current_led = start; current_led < end; current_led ++)
  {
      led_strip_dutycycle[current_led * 3] = b;
      led_strip_dutycycle[current_led * 3 + 1] = r;
      led_strip_dutycycle[current_led * 3 + 2] = g;
  }
  populate_usart_buffer(led_strip_dutycycle);
}

void MyLedStrip::strip_turnOff(void)
{
  uint8_t off_buffer[NUMBER_COLORS] = {0};
  populate_usart_buffer(off_buffer);
}

void MyLedStrip::strip_turnOn(void)
{
  populate_usart_buffer(led_strip_dutycycle);
}

void MyLedStrip::init(void)
{
  // Variables
  bool    currentOnOffValue = 0;
  uint8_t currentStateStrip = 0;
  uint8_t currentRedValue = 0;
  uint8_t currentGreenValue = 0;
  uint8_t currentBlueValue = 0;

  bool setStatus = true;

  // 1 - Initialize the led driver + other peripherals
  initStripGPIOs();
  initStripUSART();
  initLDMA();

  // 2 - Initialize the attributes
  // Update the values to match the values at the start of the application
  DeviceLayer::PlatformMgr().LockChipStack();
  LedStripServer::Instance().getStateStrip(1, &currentStateStrip);
  LedStripServer::Instance().getOnOffStrip(1, &currentOnOffValue);
  LedStripServer::Instance().getRedColor(1, &currentRedValue);
  LedStripServer::Instance().getGreenColor(1, &currentGreenValue);
  LedStripServer::Instance().getBlueColor(1, &currentBlueValue);
  chip::DeviceLayer::PlatformMgr().UnlockChipStack();

  // 3 - Set the values in the class
  setStatus |= setRedColor(currentRedValue);
  setStatus |= setGreenColor(currentGreenValue);
  setStatus |= setBlueColor(currentBlueValue);
  setStatus |= setStateStrip(currentStateStrip);
  setStatus |= setOnOffStrip(currentOnOffValue);

  if (setStatus != true) {
    SILABS_LOG("Failed to set attributes to initial values");
  }
}