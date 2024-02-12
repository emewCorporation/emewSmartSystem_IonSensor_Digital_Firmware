/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HRTBEAT_LED_Pin GPIO_PIN_13
#define HRTBEAT_LED_GPIO_Port GPIOC
#define SENSOR_LED_1_Pin GPIO_PIN_14
#define SENSOR_LED_1_GPIO_Port GPIOC
#define SENSOR_LED_2_Pin GPIO_PIN_15
#define SENSOR_LED_2_GPIO_Port GPIOC
#define CC2_ADC1_IN1_Pin GPIO_PIN_0
#define CC2_ADC1_IN1_GPIO_Port GPIOA
#define CC1_ADC1_IN2_Pin GPIO_PIN_1
#define CC1_ADC1_IN2_GPIO_Port GPIOA
#define BUTTON1_EXTI5_Pin GPIO_PIN_5
#define BUTTON1_EXTI5_GPIO_Port GPIOA
#define BUTTON1_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define BUTTON2_EXTI6_Pin GPIO_PIN_6
#define BUTTON2_EXTI6_GPIO_Port GPIOA
#define BUTTON2_EXTI6_EXTI_IRQn EXTI9_5_IRQn
#define BUTTON3_EXTI7_Pin GPIO_PIN_7
#define BUTTON3_EXTI7_GPIO_Port GPIOA
#define BUTTON3_EXTI7_EXTI_IRQn EXTI9_5_IRQn
#define INDICATION_LED_1_Pin GPIO_PIN_0
#define INDICATION_LED_1_GPIO_Port GPIOB
#define INDICATION_LED_2_Pin GPIO_PIN_1
#define INDICATION_LED_2_GPIO_Port GPIOB
#define INDICATION_LED_3_Pin GPIO_PIN_2
#define INDICATION_LED_3_GPIO_Port GPIOB
#define INDICATION_LED_4_Pin GPIO_PIN_10
#define INDICATION_LED_4_GPIO_Port GPIOB
#define INDICATION_LED_5_Pin GPIO_PIN_11
#define INDICATION_LED_5_GPIO_Port GPIOB
#define FLASH_EN_Pin GPIO_PIN_12
#define FLASH_EN_GPIO_Port GPIOB
#define FLASH_WP_Pin GPIO_PIN_10
#define FLASH_WP_GPIO_Port GPIOA
#define CC1_DETECT_EXTI11_Pin GPIO_PIN_11
#define CC1_DETECT_EXTI11_GPIO_Port GPIOA
#define CC1_DETECT_EXTI11_EXTI_IRQn EXTI15_10_IRQn
#define CC2_DETECT_EXTI12_Pin GPIO_PIN_12
#define CC2_DETECT_EXTI12_GPIO_Port GPIOA
#define CC2_DETECT_EXTI12_EXTI_IRQn EXTI15_10_IRQn
#define SPARE_TS_Pin GPIO_PIN_15
#define SPARE_TS_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
