// **************************
// *** WATER-LEVEL HANDLER Module ***

#ifndef WATERLEVEL_HANDLER_H
#define WATERLEVEL_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tm_stm32f4_hcsr04.h"
#include "float.h"
#include "math.h"

#define WATERLEVEL_SCALER 100

/*** HCSR04 structure which represents the module which measures the distance
 * through the signals ***/
TM_HCSR04_t HCSR04;

uint8_t WATERLEVEL_ALARM_MIN_THRESHOLD_REACHED;
uint8_t	WATERLEVEL_ALARM_MAX_THRESHOLD_REACHED;

float_t WATERLEVEL;

/*** Initialization of the Water Level Module***/
void WATERLEVEL_HANDLER_INIT(void);

/*** Receive the water level in centimeters (distance of the module to the water ***/
void WATERLEVEL_HANDLER_Receive(void);

/*** Print the water level variable ***/
void WATERLEVEL_HANDLER_Print(void);

#ifdef __cplusplus
}
#endif

#endif /* WATER-LEVEL_HANDLER_H */
// **************************

