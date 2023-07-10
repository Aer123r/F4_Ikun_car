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
#include "usart.h"

void Main(void);



#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/
#include "motor.hpp"
#include "encoder/encoder.hpp"
#include "driver/driver.hpp"
#include "controller/controller.hpp"
#include "servo.hpp"



#endif
#endif //REF_STM32F4_COMMON_INC_H

