/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORS_VARIABLES_H
#define __SENSORS_VARIABLES_H
#include "float.h"
#include "math.h"

#ifdef __cplusplus
 extern "C" {
#endif

// **************************************
// *** SYSTEM SENSORS VARIABLES *********
uint8_t GPS_MODULE_ON;
uint8_t	TEMPERATURE_MODULE_ON;
uint8_t	WATERLEVEL_MODULE_ON;
uint16_t SAMPLING_PERIOD;
float_t WATERTANK_HEIGHT;
float_t TEMPERATURE_MAX_THRESHOLD;
float_t TEMPERATURE_MIN_THRESHOLD;
float_t WATERLEVEL_MAX_THRESHOLD;
float_t WATERLEVEL_MIN_THRESHOLD;
// **************************************
// **************************************

#ifdef __cplusplus
}
#endif

#endif /* SENSORS_VARIABLES_H */
// **************************
