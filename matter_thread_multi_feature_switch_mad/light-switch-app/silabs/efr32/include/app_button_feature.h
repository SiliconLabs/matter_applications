#ifndef APP_BUTTON_FEATURE_H_
#define APP_BUTTON_FEATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define APP_BUTTON_FEATURE_INITIAL_PRESSED          0
#define APP_BUTTON_FEATURE_SHORT_RELEASED           1
#define APP_BUTTON_FEATURE_LONG_PRESSED             2
#define APP_BUTTON_FEATURE_LONG_RELEASED            3
#define APP_BUTTON_FEATURE_VERY_LONG_RELEASED       4
#define APP_BUTTON_FEATURE_MULTI_PRESS_ONGOING      5
#define APP_BUTTON_FEATURE_MULTI_PRESS_COMPLETE     6

#define MULTI_PRESS_DURATION (100)
#define SHORT_PRESS_DURATION (500)
#define MEDIUM_PRESS_DURATION (1000)
#define LONG_PRESS_DURATION  (3000)

void app_button_feature_init(void);

#ifdef __cplusplus
}
#endif

#endif 
