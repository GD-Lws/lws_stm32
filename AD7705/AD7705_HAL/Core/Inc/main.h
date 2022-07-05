/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
void for_delay_us(uint32_t nus);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY0_Pin GPIO_PIN_6
#define KEY0_GPIO_Port GPIOF
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOF
#define PIN_DAC_Pin GPIO_PIN_4
#define PIN_DAC_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA
#define AD7705_ADDR_Pin GPIO_PIN_4
#define AD7705_ADDR_GPIO_Port GPIOC
#define AD7705_CS_Pin GPIO_PIN_5
#define AD7705_CS_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_13
#define LED0_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */
/* 直接操作寄存器的方法控制IO*/
#define	digitalHi(p,i)			{p->BSRR=i;}			  //設置為高電平
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}	//設置為低電平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//電平反轉
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
