#ifndef __FREERTOS_H
#define __FREERTOS_H

#include "FreeRTOS.h"
#include "cmsis_os.h"
#ifdef __cplusplus
extern "C" {
#endif

// List of semaphores
extern osMutexId pwmMutexHandle;


// List of Tasks
/*--------------------------------- System Tasks -------------------------------------*/
extern osThreadId ledBlinkyTaskHandle;
extern osThreadId motorTaskHandle;
extern osThreadId obstacleTaskHandle;
/*---------------------------------- User Tasks --------------------------------------*/
// extern osThreadId_t oledTaskHandle;         // Usage: 4000 Bytes stack
// extern osThreadId_t controlLoopFixUpdateHandle;  // Usage: 4000 Bytes stack

/*---------------- 60K (used) / 64K (for FreeRTOS on ccram) ------------------*/


#ifdef __cplusplus
}
#endif

#endif
