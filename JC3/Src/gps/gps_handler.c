// **************************
// *** GPS HANDLER Module ***

#include "string.h"
#include "gps/minmea.h"
#include "gps/gps_handler.h"
#include "usart.h"
#include "main.h"

#include "stdio.h"
#include "stdlib.h"

#include "temperature/temperature_handler.h"

void GPS_HANDLER_INIT(void){
	memset(gps_buffrx, 0, GPS_BUFFRXSIZE); // RX buffer initialization
	gps_c = 0;
	gps_p = 0;
	nmea_preserved_data.valid = false;
	NMEA_MESSAGE_ARRIVED = false;
}

void GPS_HANDLER_Receive(){
	HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, &gps_c, 1, 100);
	NMEA_MESSAGE_ARRIVED = false;
	if(status != HAL_BUSY && status != HAL_ERROR){
	  if(status != HAL_TIMEOUT){
		  gps_buffrx[gps_p] = gps_c;
		  gps_p++;
	  }else{
		  if(gps_p > 0) {
			  GPS_HANDLER_Parse_Buffrx();
		  }
		  memset(gps_buffrx, 0, gps_p);
		  gps_p = 0;
	  }
	}
}

bool GPS_HANDLER_NMEA_PreservedDataReceived(void* frame, int minmea_sentence_id){

	if(minmea_sentence_id == MINMEA_SENTENCE_GGA){
		struct minmea_sentence_gga* gga = (struct minmea_sentence_gga*) frame;
		return (gga->time.hours != '\0' && gga->time.minutes != '\0' && gga->time.seconds != '\0' &&
				gga->altitude.value != '\0' && gga->latitude.value != '\0' && gga->longitude.value != '\0');

	}else if(minmea_sentence_id == MINMEA_SENTENCE_GLL){
		struct minmea_sentence_gll* gll = (struct minmea_sentence_gll*) frame;
		return (gll->time.hours != '\0' && gll->time.minutes != '\0' && gll->time.seconds != '\0' &&
				 gll->latitude.value != '\0' && gll->longitude.value != '\0');
	}

	return false;
}

void GPS_HANDLER_Parse_Buffrx(void) {

	memset(&last_valid_nmea_sentences, '\0', sizeof(last_valid_nmea_sentences));

	NMEA_MESSAGE_ARRIVED = true;

	/** Make priority on GGA message over GGL message for the NMEA preserved data **/
	bool gga_preserved_data_received_flag = false;

	//char test[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n\n$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n\n$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62\r\n\n$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74\r\n\n$GPGLL,3723.2475,N,12158.3416,W,161229.487,A,A*41\r\n\n$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48\r\n\n$GPGST,024603.00,3.2,6.6,4.7,47.3,5.8,5.6,22.0*58\r";
	//char* buffrxp = test;

	char* buffrxp = (char*) gps_buffrx;
	char* sentence = strtok(buffrxp, "\n\n");
	while (sentence) {

		uint8_t* sentencep = (uint8_t*) sentence;

		switch(minmea_sentence_id(sentence, true)){

				case MINMEA_INVALID: break;

				case MINMEA_UNKNOWN: break;

				case MINMEA_SENTENCE_GSA: {
					struct minmea_sentence_gsa frame;
					if(minmea_parse_gsa(&frame, sentence)) {
						memcpy(&last_valid_nmea_sentences.gsa, &frame, sizeof(frame));
					}
				} break;

				case MINMEA_SENTENCE_RMC: {
		            struct minmea_sentence_rmc frame;
		            if (minmea_parse_rmc(&frame, sentence)) {
		            	memcpy(&last_valid_nmea_sentences.rmc, &frame, sizeof(frame));
		            }
		        } break;

		        case MINMEA_SENTENCE_GSV: {
		            struct minmea_sentence_gsv frame;
		            if (minmea_parse_gsv(&frame, sentence)) {
		            	memcpy(&last_valid_nmea_sentences.gsv, &frame, sizeof(frame));
		            }
		        } break;

		        case MINMEA_SENTENCE_GLL: {
		        	struct minmea_sentence_gll frame;
		        	if (minmea_parse_gll(&frame, sentence)){
		        		memcpy(&last_valid_nmea_sentences.gll, &frame, sizeof(frame));

		        			// Check whether or not received time/longitude/latitude/altitude values are not-null in order to preserve it for later
		        			if(GPS_HANDLER_NMEA_PreservedDataReceived(&frame, MINMEA_SENTENCE_GLL) == true
		        					&& gga_preserved_data_received_flag == false){

		        				// reset structure prior to update values
								memset(&nmea_preserved_data, '\0', sizeof(nmea_preserved_data));
								nmea_preserved_data.valid = false;

		        				memset(&last_valid_nmea_sentences, '\0', sizeof(last_valid_nmea_sentences));
		        				memcpy(&nmea_preserved_data.time, &frame.time, sizeof(frame.time));
		        				memcpy(&nmea_preserved_data.latitude, &frame.latitude, sizeof(frame.latitude));
		        				memcpy(&nmea_preserved_data.longitude, &frame.longitude, sizeof(frame.longitude));
		        				nmea_preserved_data.valid = true;
		        			}
		        	}
		        } break;

				case MINMEA_SENTENCE_GGA: {
					struct minmea_sentence_gga frame;
					if (minmea_parse_gga(&frame, sentence)) {
						memcpy(&last_valid_nmea_sentences.gga, &frame, sizeof(frame));

						// Check whether or not received time/longitude/latitude/altitude values are not-null in order to preserve it for later
						if(GPS_HANDLER_NMEA_PreservedDataReceived(&frame, MINMEA_SENTENCE_GGA) == true){

							// reset structure prior to update values
							memset(&nmea_preserved_data, '\0', sizeof(nmea_preserved_data));
							nmea_preserved_data.valid = false;

							memcpy(&nmea_preserved_data.time, &frame.time, sizeof(frame.time));
							memcpy(&nmea_preserved_data.latitude, &frame.latitude, sizeof(frame.latitude));
							memcpy(&nmea_preserved_data.longitude, &frame.longitude, sizeof(frame.longitude));
							memcpy(&nmea_preserved_data.altitude, &frame.altitude, sizeof(frame.altitude));
							nmea_preserved_data.altitude_units = frame.altitude_units;
							nmea_preserved_data.valid = true;

							gga_preserved_data_received_flag = true;
						}
					}
				} break;

		        case MINMEA_SENTENCE_VTG: {
		        	struct minmea_sentence_vtg frame;
		        	if (minmea_parse_vtg(&frame, sentence)){
		        		memcpy(&last_valid_nmea_sentences.vtg, &frame, sizeof(frame));
		        	}
		        } break;

		        case MINMEA_SENTENCE_GST: {
		        	struct minmea_sentence_gst frame;
		        	if (minmea_parse_gst(&frame, sentence)){
		        		memcpy(&last_valid_nmea_sentences.gst, &frame, sizeof(frame));
		        	}
		        } break;
		}
		sentence[strlen(sentence)] = '\0';
		//printf("%s\r\n",sentencep);
		//HAL_UART_Transmit(&huart2, sentencep, strlen(sentence) + 1, 1000);

		sentence = strtok(NULL, "\n\n");
	}
}

void GPS_HANDLER_NMEA_Print(void){

	if(nmea_preserved_data.valid == true){

		int n;
		char nmea_output[192];
		memset(nmea_output, '\0', 192);

		int h = nmea_preserved_data.time.hours;
		int m = nmea_preserved_data.time.minutes;
		int s = nmea_preserved_data.time.seconds;

		//float lat = (float) nmea_preserved_data.latitude.value / (float) nmea_preserved_data.latitude.scale;
		//float lon = (float) nmea_preserved_data.longitude.value / (float) nmea_preserved_data.longitude.scale;

		int lat = nmea_preserved_data.latitude.value;
		int lat_scale = nmea_preserved_data.latitude.scale;

		int lon = nmea_preserved_data.longitude.value;
		int lon_scale = nmea_preserved_data.longitude.scale;

		if(nmea_preserved_data.altitude.value != '\0' && nmea_preserved_data.altitude.value != '\0'){

			//float alt = (float) nmea_preserved_data.altitude.value / (float) nmea_preserved_data.altitude.scale;
			char alt_units = nmea_preserved_data.altitude_units;

			int alt = nmea_preserved_data.altitude.value;
			int alt_scale = nmea_preserved_data.altitude.scale;

			n = sprintf(nmea_output, "[GPS Module] Time: %02d:%02d:%02d Latitude: %d/%d, Longitude: %d/%d, Altitude: %d/%d %c\r\n", h, m, s, lat, lat_scale, lon, lon_scale, alt, alt_scale, alt_units);
		}else{
			n = sprintf(nmea_output, "[GPS Module] Time: %02d:%02d:%02d Latitude: %d/%d, Longitude: %d/%d \r\n", h, m, s, lat, lat_scale, lon, lon_scale);
		}

		if(n > 0){
			printf("%s", nmea_output);
			//uint8_t* nmea_outputp = (uint8_t*) nmea_output;
			//HAL_UART_Transmit(&huart2, nmea_outputp, n, 1000);
		}else{
			printf("[GPS Module] There was a problem printing NMEA stored values...\r\n");
		}

	}else{
		printf("[GPS Module] No coordinates have been received\r\n");
	}


}

bool GPS_HANDLER_NMEA_MESSAGE_Received(){
	return NMEA_MESSAGE_ARRIVED;
}

// **************************
