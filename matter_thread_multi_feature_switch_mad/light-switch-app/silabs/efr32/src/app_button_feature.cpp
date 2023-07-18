#include "app_button_feature.h"
#include "sl_sleeptimer.h"

#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"

#include "AppTask.h"
#include <FreeRTOS.h>

typedef struct {
    uint32_t timestamp;
    uint8_t state;
    uint8_t longPressCheck = false;
    uint8_t presscount = 0;
} button_state_t;

TimerHandle_t sLongPress;
TimerHandle_t sMultiPress;

static button_state_t buttons[SL_SIMPLE_BUTTON_COUNT];

const char *str[] = {
    "inital press",
    "short release",
    "long press",
    "long release",
    "very long release",
    "multi press ongoing",
    "multi press complete"
};

void LongPressEventHandler(xTimerHandle xTimer) {

    uint8_t i = 1; // Handle for sl_button_btn1
    if (buttons[i].longPressCheck == true &&
        sl_button_get_state(SL_SIMPLE_BUTTON_INSTANCE(i)) == SL_SIMPLE_BUTTON_PRESSED) {
        buttons[i].state = APP_BUTTON_FEATURE_LONG_PRESSED;
        
        buttons[i].longPressCheck = false;
        AppTask::GetAppTask().ButtonEventHandler(SL_SIMPLE_BUTTON_INSTANCE(i), buttons[i].state);
    }
}

void MultiPressEventHandler(xTimerHandle xTimer) {

    uint8_t i = 1; // Handle for sl_button_btn1
    if (buttons[i].presscount > 0) {
        buttons[i].state = APP_BUTTON_FEATURE_MULTI_PRESS_COMPLETE;
        
        SILABS_LOG("MULTI_PRESS_COMPLETE---> btn %d: state: %s, presscount: %d",i , str[buttons[i].state],
                                                                    buttons[i].presscount);
        AppTask::GetAppTask().customButtonEventHandler(SL_SIMPLE_BUTTON_INSTANCE(i), buttons[i].state, buttons[i].presscount);
        buttons[i].presscount = 0;
    }
}

void app_button_feature_init(void)
{
    sLongPress = xTimerCreate("longPressTmr",// This is demo for button 1
                               pdMS_TO_TICKS(MEDIUM_PRESS_DURATION),// == default timer period (mS)
                               false,            // no timer reload (==one-shot)
                               (void *) 0,    // init timer id = light obj context
                               LongPressEventHandler // timer callback handler
                );
    if (sLongPress == NULL) {
        //SILABS_LOG(" Initialize Timer fails, Handle the error");
    }
    sMultiPress = xTimerCreate("lmultiPressTmr",// This is demo for button 1
                               pdMS_TO_TICKS(250),// == default timer period (mS)
                               false,            // no timer reload (==one-shot)
                               (void *) 0,    // init timer id = light obj context
                               MultiPressEventHandler // timer callback handler
                );
    if (sMultiPress == NULL) {
        //SILABS_LOG(" Initialize Timer fails, Handle the error");
    }
}

void sl_button_on_change(const sl_button_t * handle)
{
    uint32_t t_diff;    
    if (handle == SL_SIMPLE_BUTTON_INSTANCE(0)) {
        AppTask::GetAppTask().ButtonEventHandler(handle, sl_button_get_state(handle));
    } else { //handle == sl_button_btn1
        uint8_t i = 1;

        if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
            buttons[i].timestamp = sl_sleeptimer_get_tick_count();
            buttons[i].state = APP_BUTTON_FEATURE_INITIAL_PRESSED;
            buttons[i].longPressCheck = true;
            buttons[i].presscount += 1;

            // timer for long press and multi press 
            if (xTimerStart(sLongPress, 0)!=pdPASS) 
            {
                //SILABS_LOG(" Start Timer fails, Handle the error");
            }
            if (xTimerStart(sMultiPress, 0)!=pdPASS) 
            {
                //SILABS_LOG(" Start Timer fails, Handle the error");
            }

        } else if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
            t_diff = sl_sleeptimer_get_tick_count() - buttons[i].timestamp;
            
            if (t_diff < sl_sleeptimer_ms_to_tick(SHORT_PRESS_DURATION)) {
                buttons[i].state = APP_BUTTON_FEATURE_SHORT_RELEASED;
                xTimerStop(sLongPress, 0);
            } 
            else if (t_diff < sl_sleeptimer_ms_to_tick(LONG_PRESS_DURATION)) {
                buttons[i].state = APP_BUTTON_FEATURE_LONG_RELEASED;
            } else {
                buttons[i].state = APP_BUTTON_FEATURE_VERY_LONG_RELEASED;
                // if we dont handle very long released event, comment it.
                //buttons[i].state = APP_BUTTON_FEATURE_LONG_RELEASED;
            }
        }

        AppTask::GetAppTask().ButtonEventHandler(handle, buttons[i].state);
    }
}
