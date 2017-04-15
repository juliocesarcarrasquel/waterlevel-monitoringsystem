/**
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "water-level/tm_stm32f4_hcsr04.h"
#include "tim.h"
#include "main.h"

uint8_t TM_HCSR04_Init(TM_HCSR04_t* HCSR04, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin) {	
	/* Init Delay functions */
	
	/* Save everything */
	HCSR04->ECHO_GPIOx = ECHO_GPIOx;
	HCSR04->ECHO_GPIO_Pin = ECHO_GPIO_Pin;
	HCSR04->TRIGGER_GPIOx = TRIGGER_GPIOx;
	HCSR04->TRIGGER_GPIO_Pin = TRIGGER_GPIO_Pin;
	
	/* Initialize pins */	
	/* Trigger pin */
	// Change: Initialization of PIN is already done in GPIO file
	//TM_GPIO_Init(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Medium);
	
	/* Echo pin */
	// Change: Initialization of PIN is already done in GPIO file
	//TM_GPIO_Init(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Medium);
	
	/* Trigger set to low */
	HAL_GPIO_WritePin(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, RESET);
	
	/* Start measurement, check if sensor is working */
	if (TM_HCSR04_Read(HCSR04) >= 0) {
		/* Sensor OK */
		return 1;
	}
	
	/* Sensor error */
	return 0;
}

float TM_HCSR04_Read(TM_HCSR04_t* HCSR04) {
	uint32_t time, timeout;
	/* Trigger low */
	HAL_GPIO_WritePin(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, RESET);
	/* Delay 2 us */
	TIM6_MicrosecondsDelay(2);
	/* Trigger high for 10us */
	HAL_GPIO_WritePin(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, SET);
	/* Delay 10 us */
	TIM6_MicrosecondsDelay(10);
	/* Trigger low */
	HAL_GPIO_WritePin(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, RESET);
	
	/* Give some time for response */
	timeout = HCSR04_TIMEOUT;

	while(! HCSR04_READPIN(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)){
		if (timeout-- == 0x00) {
			return -1;
		}
	}
	time = 0;
	/* Wait till signal is low */
	while(HCSR04_READPIN(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)){
		time++;						/* Increase time */
	    TIM6_MicrosecondsDelay(1);		/* Delay 1us */
	}
	/* Convert us to cm */
	HCSR04->Distance =  (float)time * HCSR04_NUMBER;
	/* Return distance */
	return HCSR04->Distance;
}
