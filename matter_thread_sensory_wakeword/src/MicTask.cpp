#include <cstddef>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "AppConfig.h"
#include "semphr.h"
#include "LightSwitchMgr.h"
#include "../model/net.h"
#include "../model/search.h"
#include "sl_mic.h"
#include "sl_mic_i2s_config.h"
#include "MicTask.h"

#define MIC_TASK_MAIN_NAME "MicTaskMain"
#define MIC_TASK_MAIN_STACK_SIZE 512
#define MIC_TASK_MAIN_PRIORITY 1

#define MIC_PROCESS_BRICK_FUNCTION_NAME "ProcessBrickFunction"
#define MIC_PROCESS_BRICK_FUNCTION_STACK_SIZE 512
#define MIC_PROCESS_BRICK_FUNCTION_PRIORITY 1

#define WAKEWORD_PHRASE_LED_ON_INDEX 1
#define WAKEWORD_PHRASE_LED_OFF_INDEX 2

static TaskHandle_t MicTaskHandler;
static TaskHandle_t MicProcessHandler;
StackType_t xMicStack[ MIC_TASK_MAIN_STACK_SIZE ];
StackType_t xMicProcessStack[ MIC_PROCESS_BRICK_FUNCTION_STACK_SIZE ];
StaticTask_t xMicTaskBuffer;
StaticTask_t xMicProcessBuffer;

static SemaphoreHandle_t semaphore_handle;

int16_t MicTask::buff[2 * BRICK_SIZE_SAMPLES];

MicTask MicTask::sMicTask;

CHIP_ERROR MicTask::Init()
{
  CHIP_ERROR err = CHIP_NO_ERROR;
  int delay;
  int param_a_offset;
  const unsigned short *net;
  const unsigned short *grammar;

  delay        = 0;
  param_a_offset = 0;
  net          = dnn_wakeword_netLabel;
  grammar      = gs_wakeword_grammarLabel;

  errors_t result = SensoryInitialize((u32)net, (u32)grammar, param_a_offset, delay);
  if(result != ERR_OK)
    {
      err = MIC_INIT_FAILED;
      SILABS_LOG("MicTask::GetInstance()::Init() failed!\n");
    }

  return err;
}


const char * MicTask::GetLabel(uint16_t word_id)
{
    if(word_id < (uint16_t)WAKEWORD_PHRASE_COUNT) {
        return wakeword_phrases[word_id];
    }

    return "Unknown";
}

void MicTask::StartStreaming()
{
  uint8_t channels = 1;
  uint32_t sample_rate_hz = (1000 * BRICK_SIZE_SAMPLES) / BRICK_SIZE_MS;

 sl_mic_init(sample_rate_hz, channels);
 sl_mic_start_streaming(MicTask::buff, BRICK_SIZE_SAMPLES, MicTask::OnMicData);
}

void MicTask::ProcessBrickCallback(recognition_context_t* context)
{
  LightSwitchMgr::LightSwitchAction action;

  if(context->error == ERR_NOT_FINISHED) {
      return;
  } else if(context->error != ERR_OK) {
      SILABS_LOG("[MIC]: MicTask::ProcessBrickCallback() failed!\n");
      SILABS_LOG("[MIC]: Error: %d\n", context->error);
      return;
  }

  if(context->wordID == WAKEWORD_PHRASE_LED_ON_INDEX || context->wordID == WAKEWORD_PHRASE_LED_OFF_INDEX) {
      SILABS_LOG("[MIC]: Recognize: %s", MicTask::GetLabel(context->wordID));
      action = (context->wordID == WAKEWORD_PHRASE_LED_ON_INDEX) ? LightSwitchMgr::LightSwitchAction::On : LightSwitchMgr::LightSwitchAction::Off;
      LightSwitchMgr::GetInstance().TriggerLightSwitchAction(action);
  }
}

void MicTask::OnMicData(const void* buffer, uint32_t frames)
{
  (void)frames;
  (void)buffer;

  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

  if(semaphore_handle != NULL) {
    xSemaphoreGiveFromISR(semaphore_handle, &pxHigherPriorityTaskWoken);
  }
}

void MicTask::ProcessBrickFunction(void* buffer)
{
  while(1) {
    if(semaphore_handle != NULL) {
      if(xSemaphoreTake(semaphore_handle, pdMS_TO_TICKS(100)) == pdTRUE) {
          SensoryProcessBrick((s16*)(buffer), MicTask::ProcessBrickCallback);
          xSemaphoreGive(semaphore_handle);
      }
    }
  }
}

 void MicTask::MicTaskMain(void *pvParamaters)
 {
   (void) pvParamaters;

   if(MicTask::GetMicTask().Init() != CHIP_NO_ERROR) {
       SILABS_LOG("MicTask::MicTaskMain() failed!\n");
       appError(CHIP_ERROR_INTERNAL);
   }
   MicTask::StartStreaming();
   while(1)
     {
     }
 }

CHIP_ERROR MicTask::StartMicTask(void)
{
  CHIP_ERROR err = CHIP_NO_ERROR;
   MicTaskHandler = xTaskCreateStatic(MicTask::MicTaskMain,
                     MIC_TASK_MAIN_NAME,
                     MIC_TASK_MAIN_STACK_SIZE,
                     (void*)NULL,
                     MIC_TASK_MAIN_PRIORITY,
                     xMicStack,
                     &xMicTaskBuffer);
   if(MicTaskHandler == NULL) {
       SILABS_LOG("(MicTask::MicTaskMain() created failed!\n");
       return MIC_ERROR_CREATE_TASK_FAILED;
   }

  semaphore_handle = xSemaphoreCreateBinary();

  MicProcessHandler = xTaskCreateStatic(MicTask::ProcessBrickFunction,
              MIC_PROCESS_BRICK_FUNCTION_NAME,
              MIC_PROCESS_BRICK_FUNCTION_STACK_SIZE,
              (void *)MicTask::buff,
              MIC_PROCESS_BRICK_FUNCTION_PRIORITY,
              xMicProcessStack,
              &xMicProcessBuffer);
  if(MicProcessHandler == NULL) {
         SILABS_LOG("(MicTask::ProcessBrickFunction() created failed!\n");
         return MIC_ERROR_CREATE_TASK_FAILED;
     }
  return err;
}

