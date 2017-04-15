/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "inttypes.h"
#include "float.h"
#include "ctype.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define TEMPERATURE_MINIMUM_SUPPORTED_THRESHOLD		-55
#define TEMPERATURE_MAXIMUM_SUPPORTED_THRESHOLD		125
#define TEMPERATURE_VALID_THRESHOLD(t)				((TEMPERATURE_MINIMUM_SUPPORTED_THRESHOLD) < t && t < (TEMPERATURE_MAXIMUM_SUPPORTED_THRESHOLD))

/* Given the Water-tank with height c the maximum threshold that the user
 * can be set in order to eject an alarm is t_max = WATERLEVEL_MAX_THRESHOLD < c - 3,
 * this is because the sensor cannot measure the distance to an obstacle within a distance
 * less than 3 cm. Analogously, the minimum threshold that can be set is t_min = WATERLEVEL_MIN_THRESHOLD > c - 400,
 * because the device cannot calculate the distance to an obstacle within a distance greater than 400 cm.
 * *** In general, for any threshold t: c - 400 < t < c - 2 && t_max > t_min ***
 */
#define WATERLEVEL_MINIMUM_SUPPORTED_THRESHOLD(h)	h - 400
#define WATERLEVEL_MAXIMUM_SUPPORTED_THRESHOLD(h)	h - 2
#define WATERLEVEL_VALID_THRESHOLD(t, h)			((WATERLEVEL_MINIMUM_SUPPORTED_THRESHOLD(h)) < t && t < (WATERLEVEL_MAXIMUM_SUPPORTED_THRESHOLD(h)))

#define SENSOR_VARIABLE_UNINITIALZED				FLT_MAX
#define	DEFAULT_SAMPLINMG_PERIOD					1000
#define	DEFAULT_STATE_GPS_MODULE					1
#define DEFAULT_STATE_TEMPERATURE_MODULE			1
#define	DEFAULT_STATE_WATERLEVEL_MODULE				1

#define ESCAPE_CHARACTER(q)							(q == 0x71 || q == 0x51)

#define SCANF_BUFFR 								64
#define SETUP_COMMAND 								"SETUP"
#define START_COMMAND 								"START"
#define	CLEAN_COMMAND								"CLEAN"
#define WELCOME_MESSAGE 							"Welcome to the STM32L476RG Management Console\r\nDeveloped by: Julio César Carrasquel\r\nLa Sapienza University of Rome. INTECS Solutions SpA. Rome, Italy.\r\n"
#define SETUP_MESSAGE 								"\r\nMicrocontroller Setup Configuration:\r\n\t1. Choose sensors\r\n\t2. Choose sampling period\r\n\t3. Set Water-tank height\r\n\t4. Set Alarm thresholds\r\n\t5. Go back\r\n"
#define GPS_MODULE_ON_MESSAGE						"GPS Module [ON/OFF]: "
#define TEMPERATURE_MODULE_ON_MESSAGE				"Temperature Module [ON/OFF]: "
#define WATERLEVEL_MODULE_ON_MESSAGE				"Water-Level Module [ON/OFF]: "
#define TEMPERATURE_THRESHOLD_MAX_MESSAGE			"Temperature MAX Level [Celsius degrees]: "
#define TEMPERATURE_THRESHOLD_MIN_MESSAGE			"Temperature MIN Level [Celsius degrees]: "
#define WATERLEVEL_THRESHOLD_MAX_MESSAGE			"Water-Level MAX Level [Centimeters]: "
#define WATERLEVEL_THRESHOLD_MIN_MESSAGE			"Water-Level MIN Level [Centimeters]: "
#define SAMPLING_PERIOD_MESSAGE						"Sampling period in milliseconds [min: 1000 ms]: "
#define WATERLEVEL_TANKCAPACITY_MESSAGE				"Water-tank height (height in centimeters): "
#define OPTION_ON									"ON"
#define OPTION_OFF									"OFF"
#define PROMPT 										">"

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
