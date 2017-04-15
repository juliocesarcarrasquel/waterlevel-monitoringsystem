// **************************
// *** TEMPERATURE HANDLER Module ***

#ifndef TEMPERATURE_HANDLER_H
#define TEMPERATURE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
#include "float.h"
#include "math.h"

/*** Scaler used when converting temperature value to an integer value
 * Since the precision is fixed to 12-bits the scaler is fixed to 10000 ***/
#define TEMPERATURE_SCALER 10000

/*** 64-bits identifier of the devices ***/
uint8_t temperature_device[8];

// Auxiliary variable to read the temperature from the sensor
float_t temperature_value;

/*** OneWire structure which represents the 1-Wire channel for communicate us
 * with the Temperature sensor ***/
TM_OneWire_t OneWire1;

/** Alarms **/
uint8_t TEMPERATURE_ALARM_MIN_THRESHOLD_REACHED;
uint8_t	TEMPERATURE_ALARM_MAX_THRESHOLD_REACHED;

/*** Initialization of the Temperature Handler Module: It initializes the 1-Wire
 * interface, it gets the 64-bits sensor identifier, and it sets the temperature
 * resolution to 12-bits precision ***/
void TEMPERATURE_HANDLER_INIT(void);

/*** It receives the 12-bits value temperature via the 1-Wire interface which it is stored
 * in the temperature value variable ***/
void TEMPERATURE_HANDLER_Receive(void);

/*** Print to the standard output the last value read from the sensor ***/
void TEMPERATURE_HANDLER_Print(void);


#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_HANDLER_H */
// **************************
