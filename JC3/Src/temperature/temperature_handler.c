// **************************
// *** TEMPERATURE HANDLER Module ***

#include "temperature/temperature_handler.h"
#include "temperature/tm_stm32f4_onewire.h"
#include "temperature/tm_stm32f4_ds18b20.h"
#include "gpio.h"
#include "main.h"
#include "sensors_variables.h"

void TEMPERATURE_HANDLER_INIT(void){
	TM_OneWire_Init(&OneWire1, GPIOB, GPIO_PIN_10);
	TM_OneWire_First(&OneWire1);

	/* Get full ROM value, 8 bytes, give location of first byte where to save */
	TM_OneWire_GetFullROM(&OneWire1, temperature_device);

	TM_DS18B20_SetResolution(&OneWire1, temperature_device, TM_DS18B20_Resolution_12bits);

	TEMPERATURE_ALARM_MIN_THRESHOLD_REACHED = RESET;
	TEMPERATURE_ALARM_MAX_THRESHOLD_REACHED = RESET;
	temperature_value = 0.0;
}

void TEMPERATURE_HANDLER_Receive(void){

	TM_DS18B20_StartAll(&OneWire1);

	while (!TM_DS18B20_AllDone(&OneWire1));

	TIM6_MicrosecondsDelay(60000);

	TIM6_MicrosecondsDelay(60000);

	TM_DS18B20_Read(&OneWire1, temperature_device, &temperature_value);

	TM_DS18B20_Start(&OneWire1, temperature_device);

	TIM6_MicrosecondsDelay(60000);

	// Inspect alarms
	TEMPERATURE_ALARM_MAX_THRESHOLD_REACHED = TEMPERATURE_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && temperature_value > TEMPERATURE_MAX_THRESHOLD ? SET : RESET;
	TEMPERATURE_ALARM_MIN_THRESHOLD_REACHED = TEMPERATURE_MIN_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && temperature_value < TEMPERATURE_MIN_THRESHOLD ? SET : RESET;
}

void TEMPERATURE_HANDLER_Print(void){

	uint32_t r = temperature_value * TEMPERATURE_SCALER;

	printf("[Temperature Module] Temperature: %d C° Scale: %d\r\n", r, TEMPERATURE_SCALER);

	if(TEMPERATURE_MIN_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && TEMPERATURE_ALARM_MIN_THRESHOLD_REACHED){
		printf("[Temperature Module] [WARNING] Temperature has passed MINIMUM level!\r\n");
	}
	if(TEMPERATURE_MAX_THRESHOLD != SENSOR_VARIABLE_UNINITIALZED && TEMPERATURE_ALARM_MAX_THRESHOLD_REACHED){
		printf("[Temperature Module] [WARNING] Temperature has passed MAXIMUM level!\r\n");
	}
}

// **************************
