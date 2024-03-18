/***************************************************************************//**
 * @file led_strip_driver.h
 * @brief Led Strip header file
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

#ifndef LED_STRIP_DRIVER_H_
#define LED_STRIP_DRIVER_H_

#include <stdint.h>

#define NUMBER_COLORS NUMBER_LEDS * 3


class MyLedStrip
{
public:
    // ENUM
    enum action_t {
        MOVE_TO_STATE = 0,
        MOVE_TO_RED,
        MOVE_TO_GREEN,
        MOVE_TO_BLUE,
        MOVE_TO_ONOFF,
        INVALID_ACTION,
        IGNORE_ACTION
    } Action;
    
    void getRGBColor(uint8_t* r, uint8_t* g, uint8_t* b);

    uint8_t getRedColor(void);
    uint8_t getGreenColor(void);
    uint8_t getBlueColor(void);
    uint8_t getStateStrip(void);
    bool    getOnOffStrip(void);

    bool setRedColor(uint8_t r);
    bool setGreenColor(uint8_t g);
    bool setBlueColor(uint8_t b);
    bool setStateStrip(uint8_t state);
    bool setOnOffStrip(bool onOffValue);

    void init(void); // Function to initialize the attributes and everything else
    // Function to handle changes on the cluster (Actions/Commands)
    bool InitiateActionStrip(action_t action, uint16_t endpoint, uint8_t value);
    
private:
    // Private functions
    void strip_turnOn(void);
    void strip_turnOff(void);

    void setColor(uint8_t r, uint8_t g, uint8_t b, uint32_t start, uint32_t end);

    void resetBuffer(void);
    // Arguments
    uint8_t redValue;
    uint8_t greenValue;
    uint8_t blueValue;
    uint8_t stripState;
    bool onOffState;
};

#endif /* LED_STRIP_DRIVER_H_ */
