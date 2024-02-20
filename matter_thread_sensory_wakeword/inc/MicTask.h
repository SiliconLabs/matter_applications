#ifndef MIC_TASK_H_
#define MIC_TASK_H_

#include "sensory.h"
#include "stdint.h"
#include <lib/core/CHIPError.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIC_ERROR_CREATE_TASK_FAILED CHIP_APPLICATION_ERROR(0x20)
#define MIC_INIT_FAILED CHIP_APPLICATION_ERROR(0x21)

class MicTask {
  public:
  MicTask() = default;

  static MicTask & GetMicTask() { return sMicTask; }
  /**
   * @brief Start mic task
   *
   * @return CHIP_ERROR
   */
  CHIP_ERROR StartMicTask(void);
/**
 * @brief MicTaskMain task main loop fuction
 * 
 * @param pvParamaters FreeRTOS task parameter
 */
  static void MicTaskMain(void *pvParamaters);
  /**
   * @brief Start streaming function
   * 
   */
  static void StartStreaming();
/**
 * @brief Get the wake word label
 * 
 * @param word_id Wake word index in array
 * @return const char* Wake word on string format
 */
  static const char * GetLabel(uint16_t word_id);
/**
 * @brief Give semaphore from DMA ISR
 * 
 * @param buffer Pointer to the sample buffer
 * @param freams Number of audio frames in the sample buffer
 */
  static void OnMicData(const void *buffer, uint32_t freams);
  /**
   * @brief Process brick task to process mic data buffer
   * 
   * @param buffer Mic data buffer
   */
  static void ProcessBrickFunction(void* buffer);
  /**
   * @brief Callback function to trigger switch action
   * 
   * @param context Contain result recognition after processing mic data
   */
  static void ProcessBrickCallback(recognition_context_t* context);

  private:
  static int16_t buff[2 * BRICK_SIZE_SAMPLES];
  static MicTask sMicTask;
  /**
   * @brief Mic intialization function
   * 
   * @return CHIP_ERROR 
   */
  CHIP_ERROR Init();
};

#ifdef __cplusplus
}
#endif

#endif /* MIC_TASK_H_ */
