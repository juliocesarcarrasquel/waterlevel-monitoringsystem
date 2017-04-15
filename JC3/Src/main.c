/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "sensors_variables.h"
#include "gps/gps_handler.h"
#include "temperature/temperature_handler.h"
#include "water-level/waterlevel_handler.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

__IO ITStatus interrupt_flag = RESET;
uint8_t buff[SCANF_BUFFR];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void _invalid();
void _prompt();
void _scanf(uint8_t* c);
void _touppercase(uint8_t* buff);
void INITIALIZE_SENSOR_VARIABLES();
void INITIALIZE_SENSOR_HANDLERS();
void DISPLAY_MENU();
void SETUP();
void SETUP_SENSORS();
void SETUP_SAMPLING_PERIOD();
void SETUP_ALARMS();
void SETUP_WATERTANK_HEIGHT();
void SETUP_SUMMARY();
void RECEIVE_AND_PRINT_TEMPERATURE_AND_WATERLEVEL();
void RECEIVE_DATA_FROM_SENSORS();

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  /* *** Start to run the counter of the 1 Mhz-Timer ***
   * This timer is used for the Temperature and the Water-Level modules *** */
  HAL_TIM_Base_Start(&htim6);

  INITIALIZE_SENSOR_VARIABLES();

  /* USER CODE END 2 */

  /* Infinite loop */

  /* USER CODE BEGIN WHILE */

  while(1){
	  DISPLAY_MENU();

	  INITIALIZE_SENSOR_HANDLERS();

	  uint8_t esc = 0x00;
	  while(1){
		  HAL_UART_Receive_IT(&huart2, &esc, 1);
		  if(interrupt_flag){
			   interrupt_flag = RESET;
			   if(ESCAPE_CHARACTER(esc)){
				   HAL_UART_Abort_IT(&huart2);
				   break;
			   }
		  }
		  RECEIVE_DATA_FROM_SENSORS();
	  }
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure LSE Drive Capability 
    */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /**Enable MSI Auto calibration 
    */
  HAL_RCCEx_EnableMSIPLLMode();

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/***
 *
 */

PUTCHAR_PROTOTYPE {
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void RECEIVE_DATA_FROM_SENSORS(){
	if(GPS_MODULE_ON){
		/*** Case #1: Since GPS is activated, temperature and water-level measurements
		 * are synchronized with the arrival of NMEA messages ***/
		GPS_HANDLER_Receive();
		if(GPS_HANDLER_NMEA_MESSAGE_Received() == true){
			GPS_HANDLER_NMEA_Print();

			RECEIVE_AND_PRINT_TEMPERATURE_AND_WATERLEVEL();

			HAL_Delay(SAMPLING_PERIOD - 999);
			MX_USART1_UART_Init();
		}
	}else{
		/*** Case #2: GPS is not activated, let temperature and water-level modules
		 * operate at their configured sampling period ***/
		RECEIVE_AND_PRINT_TEMPERATURE_AND_WATERLEVEL();
		HAL_Delay(SAMPLING_PERIOD);
	}
}

void RECEIVE_AND_PRINT_TEMPERATURE_AND_WATERLEVEL(){
	 if(TEMPERATURE_MODULE_ON){
		 TEMPERATURE_HANDLER_Receive();
		 TEMPERATURE_HANDLER_Print();
		 HAL_Delay(50);
	 }
	 if(WATERLEVEL_MODULE_ON){
		 WATERLEVEL_HANDLER_Receive();
		 WATERLEVEL_HANDLER_Print();
	 }
}

void INITIALIZE_SENSOR_HANDLERS(){
	  if(GPS_MODULE_ON){
		  GPS_HANDLER_INIT();
	  }
	  if(TEMPERATURE_MODULE_ON){
		  TEMPERATURE_HANDLER_INIT();
	  }
	  if(WATERLEVEL_MODULE_ON){
		  WATERLEVEL_HANDLER_INIT();
	  }
}

void INITIALIZE_SENSOR_VARIABLES(){
	GPS_MODULE_ON = DEFAULT_STATE_GPS_MODULE;
	TEMPERATURE_MODULE_ON = DEFAULT_STATE_TEMPERATURE_MODULE;
	WATERLEVEL_MODULE_ON = DEFAULT_STATE_WATERLEVEL_MODULE;
	SAMPLING_PERIOD = DEFAULT_SAMPLINMG_PERIOD;
	WATERTANK_HEIGHT = SENSOR_VARIABLE_UNINITIALZED;
	TEMPERATURE_MAX_THRESHOLD = SENSOR_VARIABLE_UNINITIALZED;
	TEMPERATURE_MIN_THRESHOLD = SENSOR_VARIABLE_UNINITIALZED;
	WATERLEVEL_MAX_THRESHOLD = SENSOR_VARIABLE_UNINITIALZED;
	WATERLEVEL_MIN_THRESHOLD = SENSOR_VARIABLE_UNINITIALZED;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: transfer complete */
	interrupt_flag = SET;
}

void _invalid(){
	printf("Invalid choice. Please verify.\r\n");
}

void _prompt(){
	HAL_UART_Transmit(&huart2, PROMPT, 1, HAL_MAX_DELAY);
}

void _choose_option(){
	HAL_UART_Transmit(&huart2, (uint8_t*) "Choose your option: ", strlen("Choose your option: "), HAL_MAX_DELAY);
}

void _scanf(uint8_t* buffp){
	memset(buffp, 0, SCANF_BUFFR);
	uint8_t* p = buffp;
	uint8_t c = 0x00;
	while(1){
		HAL_UART_Receive(&huart2, (uint8_t*) &c, 1, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) &c, 1, HAL_MAX_DELAY);
		*p++ = c == 0x0D ? 0x00 : c;
		if(c == 0x0D){
			break;
		}
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

void _touppercase(uint8_t* buff){
	uint8_t* p = buff;
	while(*p){
		*p = (uint8_t) toupper(*p);
		p++;
	}
}

void DISPLAY_MENU(){
	printf("\033[0;0H\033[2J");
	printf("******************************************************************\r\n");
	printf(WELCOME_MESSAGE);
	printf("******************************************************************\r\n");
	while(1){
		_prompt();
		_scanf((uint8_t*) buff);
		_touppercase(buff);
		if(!strcmp(SETUP_COMMAND, buff)){
			SETUP();
			continue;
		}
		if(!strcmp(START_COMMAND, buff)){
			printf("\r\n******************************************************************\r\n");
			printf("Executing sensors. . . . . .\r\n\r\n");
			break;
		}
		if(!strcmp(CLEAN_COMMAND, buff)){
			printf("\033[0;0H\033[2J");
			printf("******************************************************************\r\n");
			printf(WELCOME_MESSAGE);
			printf("******************************************************************\r\n");
			continue;
		}
		_invalid();
	}
}

void SETUP(){
	while(1){
		printf(SETUP_MESSAGE);
		_choose_option();
		_scanf(buff);
		uint8_t option = (uint8_t) buff[0];
		if(strlen(buff) > 1 || option <= 0x30 || option >= 0x36){
			_invalid();
		}else{
			switch(option){
				case 0x31:	SETUP_SENSORS();
							break;
				case 0x32:	SETUP_SAMPLING_PERIOD();
							break;
				case 0x33:	SETUP_WATERTANK_HEIGHT();
							break;
				case 0x34:	SETUP_ALARMS();
							break;
			}
			if(option == 0x35){
				SETUP_SUMMARY();
				break;
			}
		}
	}
}

void SETUP_SUMMARY(){
	printf("\r\nSensors setup summary (non-configured values has been set with default values)");
	printf("\r\nGPS Module will be turned %s!...\r\n", GPS_MODULE_ON ? OPTION_ON : OPTION_OFF);
	printf("Temperature Module will be turned %s!...\r\n", TEMPERATURE_MODULE_ON ? OPTION_ON : OPTION_OFF);
	printf("Water-Level will be turned %s!...\r\n", WATERLEVEL_MODULE_ON ? OPTION_ON : OPTION_OFF);
	printf("Sampling period has been set to %d milliseconds!...\r\n", SAMPLING_PERIOD);

	if(WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED){
		uint32_t watertank_height = (uint32_t) (WATERTANK_HEIGHT * WATERLEVEL_SCALER);
		printf("Water-tank capacity (height on centimeters) set on %d cm scaler: %d \r\n", watertank_height, WATERLEVEL_SCALER);
	}

	if(TEMPERATURE_MODULE_ON == SET && TEMPERATURE_MIN_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && TEMPERATURE_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED){
		int32_t temperature_min = (int32_t) (TEMPERATURE_MIN_THRESHOLD * TEMPERATURE_SCALER);
		int32_t temperature_max = (int32_t) (TEMPERATURE_MAX_THRESHOLD * TEMPERATURE_SCALER);
		printf("Temperature thresholds set on min: %d max: %d celsius degrees scaler: %d\r\n", temperature_min, temperature_max, TEMPERATURE_SCALER);
	}

	if(WATERLEVEL_MODULE_ON == SET && WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED && WATERLEVEL_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED){
		uint32_t waterlevel_min = (uint32_t) (WATERLEVEL_MIN_THRESHOLD * WATERLEVEL_SCALER);
		uint32_t waterlevel_max = (uint32_t) (WATERLEVEL_MAX_THRESHOLD * WATERLEVEL_SCALER);
		printf("Water-level thresholds set on min: %d max: %d centimeters scaler: %d\r\n", waterlevel_min, waterlevel_max, WATERLEVEL_SCALER);
	}
	printf("\r\n");
}

void SETUP_SENSORS(){
	while(1){
		while(1){
			HAL_UART_Transmit(&huart2, GPS_MODULE_ON_MESSAGE, strlen(GPS_MODULE_ON_MESSAGE), HAL_MAX_DELAY);
			_scanf(buff);
			if( !strcmp(buff, OPTION_ON) || !strcmp(buff, OPTION_OFF)){
				GPS_MODULE_ON = !strcmp(buff, OPTION_ON) ? SET : RESET;
				break;
			}
			_invalid();
		}
		while(1){
			HAL_UART_Transmit(&huart2, TEMPERATURE_MODULE_ON_MESSAGE, strlen(TEMPERATURE_MODULE_ON_MESSAGE), HAL_MAX_DELAY);
			_scanf(buff);
			if( !strcmp(buff, OPTION_ON) || !strcmp(buff, OPTION_OFF)){
				TEMPERATURE_MODULE_ON = !strcmp(buff, OPTION_ON) ? SET : RESET;
				break;
			}
			_invalid();
		}
		while(1){
			HAL_UART_Transmit(&huart2, WATERLEVEL_MODULE_ON_MESSAGE, strlen(WATERLEVEL_MODULE_ON_MESSAGE), HAL_MAX_DELAY);
			_scanf(buff);
			if( !strcmp(buff, OPTION_ON) || !strcmp(buff, OPTION_OFF)){
				WATERLEVEL_MODULE_ON = !strcmp(buff, OPTION_ON) ? SET : RESET;
				break;
			}
			_invalid();
		}
		if(GPS_MODULE_ON == SET || TEMPERATURE_MODULE_ON == SET || WATERLEVEL_MODULE_ON == SET){
			break;
		}
		_invalid(); // At least one module must be ON
	}

	printf("\r\nGPS Module will be turned %s!...\r\n", GPS_MODULE_ON ? OPTION_ON : OPTION_OFF);
	printf("Temperature Module will be turned %s!...\r\n", TEMPERATURE_MODULE_ON ? OPTION_ON : OPTION_OFF);
	printf("Water-Level will be turned %s!...\r\n", WATERLEVEL_MODULE_ON ? OPTION_ON : OPTION_OFF);
}

void SETUP_SAMPLING_PERIOD(){
	while(1){
		HAL_UART_Transmit(&huart2, (uint8_t*)SAMPLING_PERIOD_MESSAGE, strlen(SAMPLING_PERIOD_MESSAGE), HAL_MAX_DELAY);
		_scanf(buff);
		SAMPLING_PERIOD = (uint16_t) strtoul(buff, 0x00, 10);
		if(SAMPLING_PERIOD >= 1000){
			break;
		}
		_invalid();
	}
	printf("\r\n Sampling period has been set to %d milliseconds!...\r\n", SAMPLING_PERIOD);
}

void SETUP_WATERTANK_HEIGHT(){
	while(1){
		HAL_UART_Transmit(&huart2, (uint8_t*)WATERLEVEL_TANKCAPACITY_MESSAGE, strlen(WATERLEVEL_TANKCAPACITY_MESSAGE), HAL_MAX_DELAY);
		_scanf(buff);
		WATERTANK_HEIGHT = (float_t) strtof(buff, 0x00);
		if(WATERTANK_HEIGHT > 0){
			break;
		}
		_invalid();
	}

	uint32_t watertank_height = (uint32_t) (WATERTANK_HEIGHT * WATERLEVEL_SCALER);
	printf("Water-tank capacity (height on centimeters) set on %d cm scaler: %d \r\n", watertank_height, WATERLEVEL_SCALER);
}

void SETUP_ALARMS(){

	if(TEMPERATURE_MODULE_ON == RESET && WATERLEVEL_MODULE_ON == RESET){
		HAL_UART_Transmit(&huart2, (uint8_t*) "Please activate the temperature or/and water-level module\r\n", sizeof("Please activate the temperature or/and water-level module\r\n"), HAL_MAX_DELAY);
		return;
	}

	if(TEMPERATURE_MODULE_ON == SET){
		while(1){
			HAL_UART_Transmit(&huart2, (uint8_t*)TEMPERATURE_THRESHOLD_MIN_MESSAGE, strlen(TEMPERATURE_THRESHOLD_MIN_MESSAGE), HAL_MAX_DELAY);
			_scanf(buff);
			TEMPERATURE_MIN_THRESHOLD = (float_t) strtof(buff, 0x00);
			HAL_UART_Transmit(&huart2, (uint8_t*)TEMPERATURE_THRESHOLD_MAX_MESSAGE, strlen(TEMPERATURE_THRESHOLD_MAX_MESSAGE), HAL_MAX_DELAY);
			_scanf(buff);
			TEMPERATURE_MAX_THRESHOLD = (float_t) strtof(buff, 0x00);
			if(TEMPERATURE_MAX_THRESHOLD > TEMPERATURE_MIN_THRESHOLD && TEMPERATURE_VALID_THRESHOLD(TEMPERATURE_MAX_THRESHOLD) && TEMPERATURE_VALID_THRESHOLD(TEMPERATURE_MIN_THRESHOLD)){
				break;
			}
			_invalid();
		}
	}
	if(WATERLEVEL_MODULE_ON == SET){
		if(WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED){
			while(1){
				HAL_UART_Transmit(&huart2, (uint8_t*)WATERLEVEL_THRESHOLD_MIN_MESSAGE, strlen(WATERLEVEL_THRESHOLD_MIN_MESSAGE), HAL_MAX_DELAY);
				_scanf(buff);
				WATERLEVEL_MIN_THRESHOLD = (float_t) strtof(buff, 0x00);
				HAL_UART_Transmit(&huart2, (uint8_t*)WATERLEVEL_THRESHOLD_MAX_MESSAGE, strlen(WATERLEVEL_THRESHOLD_MAX_MESSAGE), HAL_MAX_DELAY);
				_scanf(buff);
				WATERLEVEL_MAX_THRESHOLD = (float_t) strtof(buff, 0x00);
				if(WATERLEVEL_MIN_THRESHOLD >= 0 && WATERLEVEL_MAX_THRESHOLD > 0
						&& WATERLEVEL_VALID_THRESHOLD(WATERLEVEL_MIN_THRESHOLD, WATERTANK_HEIGHT)
						&& WATERLEVEL_VALID_THRESHOLD(WATERLEVEL_MAX_THRESHOLD, WATERTANK_HEIGHT)){
					break;
				}
				_invalid();
			}
		}else{
			HAL_UART_Transmit(&huart2, (uint8_t*)"To establish water-level thresholds please set water-tank capacity.\r\n", strlen((uint8_t*)"To establish water-level thresholds please set water-tank capacity.\r\n"), HAL_MAX_DELAY);
		}
	}

	if(TEMPERATURE_MODULE_ON == SET){
		int32_t temperature_min = (int32_t) (TEMPERATURE_MIN_THRESHOLD * TEMPERATURE_SCALER);
		int32_t temperature_max = (int32_t) (TEMPERATURE_MAX_THRESHOLD * TEMPERATURE_SCALER);
		printf("Temperature thresholds set on min: %d max: %d celsius degrees scaler: %d\r\n", temperature_min, temperature_max, TEMPERATURE_SCALER);
	}

	if(WATERLEVEL_MODULE_ON == SET && WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED){
		uint32_t waterlevel_min = (uint32_t) (WATERLEVEL_MIN_THRESHOLD * WATERLEVEL_SCALER);
		uint32_t waterlevel_max = (uint32_t) (WATERLEVEL_MAX_THRESHOLD * WATERLEVEL_SCALER);
		printf("Water-level thresholds set on min: %d max: %d centimeters scaler: %d\r\n", waterlevel_min, waterlevel_max, WATERLEVEL_SCALER);
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
