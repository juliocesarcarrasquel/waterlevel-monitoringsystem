// **************************
// *** WATER-LEVEL HANDLER Module ***

#include "water-level/waterlevel_handler.h"
#include "water-level/tm_stm32f4_hcsr04.h"
#include "tim.h"
#include "gpio.h"
#include "main.h"
#include "sensors_variables.h"

/*** Initialization of the Water Level Module***/
void WATERLEVEL_HANDLER_INIT(void) {
	/* Initialize distance sensor on pins:
	   *  ECHO: PC7 (D9)
	   *  TRIGGER: PB6 (D10) */
	  TM_HCSR04_Init(&HCSR04, GPIOC, GPIO_PIN_7, GPIOB, GPIO_PIN_6);

	  WATERLEVEL_ALARM_MIN_THRESHOLD_REACHED = RESET;
	  WATERLEVEL_ALARM_MAX_THRESHOLD_REACHED = RESET;
	  WATERLEVEL = 0.0;
}

/*** Receive the water level in centimeters (distance of the module to the water ***/
void WATERLEVEL_HANDLER_Receive(void) {
	TM_HCSR04_Read(&HCSR04);

	WATERLEVEL = WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED ? WATERTANK_HEIGHT - (float_t) HCSR04.Distance : (float_t) HCSR04.Distance;

	if(WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED){
		WATERLEVEL_ALARM_MIN_THRESHOLD_REACHED = WATERLEVEL_MIN_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && WATERLEVEL < WATERLEVEL_MIN_THRESHOLD ? SET : RESET;
		WATERLEVEL_ALARM_MAX_THRESHOLD_REACHED = WATERLEVEL_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && WATERLEVEL > WATERLEVEL_MAX_THRESHOLD ? SET : RESET;
	}
}

/*** Print the water level variable (and possible alarms)***/
void WATERLEVEL_HANDLER_Print(void) {
	uint32_t r = (uint32_t) (WATERLEVEL * WATERLEVEL_SCALER);
	printf("[Water-Level Module] Level from the top: %d cm Scale: %d\r\n", r, WATERLEVEL_SCALER);

	if(WATERTANK_HEIGHT != SENSOR_VARIABLE_UNINITIALZED){
		if(WATERLEVEL_MIN_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && WATERLEVEL_ALARM_MIN_THRESHOLD_REACHED){
			printf("[Water-Level Module] [WARNING] Water-level has passed MINIMUM level!\r\n");
		}
		if(WATERLEVEL_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && WATERLEVEL_ALARM_MAX_THRESHOLD_REACHED){
			printf("[Water-Level Module] [WARNING] Water-level has passed MAXIMUM level!\r\n");
		}
	}
}

// **************************
