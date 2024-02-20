/***************************************************************************//**
 * @file sensory.h
 * @brief Sensory library definitions and API,
 *        based on Sensory headers and functions exposed in the app example.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SENSORY_H
#define SENSORY_H

#ifdef __cplusplus
extern "C" {
#endif
/***************************************************************************//**
 * The size of each audio window in milliseconds.
 * The Sensory library was precompiled with this window length,
 * so this value should not be changed.
 ******************************************************************************/
#define BRICK_SIZE_MS (15)
#define BRICK_SIZE_SAMPLES (BRICK_SIZE_MS*16)

/***************************************************************************//**
 * Type aliases for legibility.
 ******************************************************************************/
typedef unsigned short int  u16;
typedef unsigned long int   u32;
typedef unsigned long long  u64;
typedef short int           s16;
typedef long int            s32;
typedef signed long long    s64;


/***************************************************************************//**
 * Sensory error codes.
 ******************************************************************************/
typedef unsigned short int errors_t;

//-- 0x, FX: Generic errors
#define ERR_OK                          0x00    // generic pass return
#define ERR_NOT_OK                      0x01    // generic error return
#define ERR_MEMORY_CORRUPT              0xFB    // RAM apparently changed -- RESERVED FOR NXP COMPATIBILITY
#define ERR_NULL_POINTER                0xFC    // fatal null pointer
#define ERR_NOT_FINISHED                0xFD    // non-fatal, need more data 
#define ERR_HEAP_FULL                   0xFE    // no room left in heap
#define ERR_LICENSE 0xFF // license isn't valid or event limit reached

//-- 1x, 2x: Data collection errors
#define ERR_DATACOL_BASE                0x10
#define ERR_DATACOL_TIMEOUT             ERR_DATACOL_BASE+0x01 // no utterance detected
#define ERR_DATACOL_TOO_SHORT           ERR_DATACOL_BASE+0x03 // utterance was too short
#define ERR_DATACOL_TOO_SOFT            ERR_DATACOL_BASE+0x04 // utterance was too soft

//-- 3x,4x: Recognition errors
#define ERR_RECOG_BASE                  0x30
#define ERR_RECOG_FAIL                  ERR_RECOG_BASE+0x01 // recognition failed
#define ERR_RECOG_LOW_CONF              ERR_RECOG_BASE+0x02 // recognition result doubtful
#define ERR_RECOG_MID_CONF              ERR_RECOG_BASE+0x03 // recognition result maybe

//-- 5x: T2SI errors
#define ERR_T2SI_BASE                   0x50
#define ERR_T2SI_PSTORE                 ERR_T2SI_BASE       // null persistent storage pointer                                                 
#define ERR_T2SI_BAD_VERSION            ERR_T2SI_BASE+0x01  // grammar version not supported
#define ERR_T2SI_NN_BAD_VERSION         ERR_T2SI_BASE+0x02  // net version not supported
#define ERR_T2SI_BAD_SETUP              ERR_T2SI_BASE+0x03  // gram or net not specified 
#define ERR_T2SI_TRIG_NOTA              ERR_T2SI_BASE+0x04  // trigger NOTA - continues
#define ERR_T2SI_NN_MISMATCH            ERR_T2SI_BASE+0x05  // gram/net mismatch(# of models)
#define ERR_T2SI_TOO_MANY_RESULTS         ERR_T2SI_BASE+0x06// MAX_RESULTS is too small
#define ERR_T2SI_UNEXPECTED_NUM_EXTRA_MODELS  ERR_T2SI_BASE+0x07  // too many extra number of models
#define ERR_T2SI_UNEXPECTED_NUM_EXTRA_GRAMS   ERR_T2SI_BASE+0x08  // too many extra number of grammars
#define ERR_T2SI_UNEXPECTED_NUM_EXTRA_NETS    ERR_T2SI_BASE+0x09  // too many extra number of nets
#define ERR_T2SI_UNEXPECTED_MFCC_TYPE     ERR_T2SI_BASE+0x0A  // The MFCC type is not supported

//-- 6x: DNN errors
#define ERR_DNN_BASE                    0x60
#define ERR_DNN_BAD_VERSION             ERR_DNN_BASE+0x00  // dnn not right format/version
#define ERR_DNN_TOO_MANY_NETS           ERR_DNN_BASE+0x01  // too many dnn nets (> MAX_DNN_NETS)
#define ERR_DNN_BAD_FORMAT              ERR_DNN_BASE+0x02
#define ERR_DNN_SV_OR_NNPQ_MISMATCH       ERR_DNN_BASE+0x03  // grammar for nnSv is not SV; or NNPQ is not supported in this version


/***************************************************************************//**
 * Inference result callback data.
 ******************************************************************************/
typedef struct recognition_context {
  u64 timestep; // index of frame
  u16 duration; // length of utterance in 15ms frames
  u16 wordID;
  u16 finalScore;
  u16 nnpqThreshold;
  errors_t error;
  int nnpqScore;
} recognition_context_t;


/***************************************************************************//**
 * Inference result callback.
 * This is invoked for each call to \ref SensoryProcessBrick
 ******************************************************************************/
typedef void (*process_brick_callback_t)(recognition_context_t* context);

/***************************************************************************//**
 * Allocate memory and initialize recognizer for performing inference.
 ******************************************************************************/
errors_t SensoryInitialize(u32 net, u32 grammar, int paramA, int delay);

/***************************************************************************//**
 * Allocate memory and initialize recognizer for performing inference.
 ******************************************************************************/
errors_t SensoryProcessBrick(s16 *brick, process_brick_callback_t callback);

/***************************************************************************//**
 * Frees allocated memory.
 ******************************************************************************/
errors_t SensoryTerminate();

#ifdef __cplusplus
}
#endif
#endif  // SENSORY_H
