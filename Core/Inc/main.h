/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define MOTOR1_A_PIN  GPIO_PIN_14
#define MOTOR1_A_PORT GPIOD

#define MOTOR1_B_PIN  GPIO_PIN_9
#define MOTOR1_B_PORT GPIOD

#define MOTOR2_A_PIN  GPIO_PIN_15
#define MOTOR2_A_PORT GPIOE

#define MOTOR2_B_PIN  GPIO_PIN_8
#define MOTOR2_B_PORT GPIOD

#define MOTOR3_A_PIN  GPIO_PIN_10
#define MOTOR3_A_PORT GPIOE

#define MOTOR3_B_PIN  GPIO_PIN_13
#define MOTOR3_B_PORT GPIOE

#define MOTOR4_A_PIN  GPIO_PIN_12
#define MOTOR4_A_PORT GPIOE

#define MOTOR4_B_PIN  GPIO_PIN_14
#define MOTOR4_B_PORT GPIOE

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
