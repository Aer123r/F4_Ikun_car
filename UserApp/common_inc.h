#ifndef REF_STM32F4_COMMON_INC_H
#define REF_STM32F4_COMMON_INC_H

#define CONFIG_FW_VERSION 1.0

/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "freertos_inc.h"
#include "tim.h"

void Main(void);

extern uint64_t serialNumber;
extern char serialNumberStr[13];


#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/
#include "motor.hpp"

#endif
#endif //REF_STM32F4_COMMON_INC_H

