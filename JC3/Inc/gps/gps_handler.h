// **************************
// *** GPS HANDLER Module ***

#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "minmea.h"
#include "usart.h"
#include "main.h"

#define GPS_BUFFRXSIZE 8192

/*** **************************************************** ***/
/*** Last set of valid sentences received by NMEA signals ***/
typedef struct {
	struct minmea_sentence_gga gga;
	struct minmea_sentence_gsa gsa;
	struct minmea_sentence_gsv gsv;
	struct minmea_sentence_gll gll;
	struct minmea_sentence_rmc rmc;
	struct minmea_sentence_vtg vtg;
	struct minmea_sentence_gst gst;
} nmea_sentences;
nmea_sentences last_valid_nmea_sentences;

/*** **************************************************** ***/
/*** NMEA Preserved data: It refers to the last valid "important" values
 *   read from the NMEA signals. For example, if in a subsequent NMEA signal
 *   does not arrive the latitude and the longitude information, their last non-null
 *   values are going to be overwritten with null. However, those previous
 *   non-null values are going to be preserved on this structure. Thereby, by means of
 *   this structure we keep track of for example which was the last position of the device. ***/
typedef struct {
	struct minmea_time time;
	struct minmea_float latitude;
	struct minmea_float longitude;
	struct minmea_float altitude;
	char altitude_units;
	bool valid;
} _nmea_preserved_data;
_nmea_preserved_data nmea_preserved_data;

// *** Buffer dedicated to receive GPS data ***
uint8_t gps_buffrx[GPS_BUFFRXSIZE];

uint8_t gps_c;	// Auxiliary variable to receive next byte
uint32_t gps_p;	// Number of bytes so far received

// TESTING PURPOSES
uint32_t gps_timeoutcounter;

bool NMEA_MESSAGE_ARRIVED;

/*** Initialization of the GPS Handler Module ***/
void GPS_HANDLER_INIT(void);

/*** Procedure to receive next byte, if the timeout to receive the next byte expires (100ms)
 * it is assume that all sentences of the corresponding NMEA message has been received and therefore
 * we proceed to parse message. Then, it is reseted the reception buffer in order to allocate
 * the following NMEA message***/
void GPS_HANDLER_Receive(void);

/*** Parsing of the received sentences within the NMEA message ***/
void GPS_HANDLER_Parse_Buffrx(void);

/*** This function checks whether or not within the NMEA message it has been arrived the
 *   data which we do not want to be reset by further calls. That is for example to keep track of
 *   values such as latitude, longitude and altitude. */
bool GPS_HANDLER_NMEA_PreservedDataReceived(void* frame, int minmea_sentence_id);

/***
 * 	Print Longitude, Latitude and Altitude.
 */
void GPS_HANDLER_NMEA_Print(void);

/***
 * Print detailed NMEA detailed message
 */
void GPS_HANDLER_NMEA_Print_Detailed_Data(void);

bool GPS_HANDLER_NMEA_MESSAGE_Received();

#ifdef __cplusplus
}
#endif

#endif /* GPS_HANDLER_H */
// **************************
