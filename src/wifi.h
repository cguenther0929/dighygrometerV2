/******************************************************************************
*	FILE: wifi.h
*
*	PURPOSE: Header file for WiFi capabilities.
*
*	AUTHOR: Clinton Guenther
*
*	TODO:	
*
******************************************************************************/
#ifndef __WIFI_H
#define __WIFI_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

#define LF			            1
#define CR			            2

#define ESP_JOIN_NETWORK_BASE_CMD           "AT+CWJAP="
#define WIFI_ROUTER_SSID                    "CJG_GbE_2G4"
#define WIFI_ROUTER_PASSWORD                "GlockHK23"

#define ESP_CIPMUX_BASE_CMD                 "CIPMUX=1"
#define ESP_CIPMUX_SINGLE_CONNECTION        "0"         // Must be strings
#define ESP_CIPMUX_MULTIPLE_CONNECTION      "1"

#define ESP_AP_OR_CLIENT_BASE_CMD           "AT+CWMODE="
#define ESP_CLIENT_MODE                     "1"
#define ESP_AP_MODE                         "2"
#define ESP_AP_AND_CLIENT_MODE              "3"


/*
 * Function: void SetEspMode (uint8_t mode)
 * --------------------
 * Define whether the module 
 * is restriced to a single connection,
 * or it multiple connections are allowed 
 *
 * returns: True if successful
 */
bool SetEspCipmuxMode (const char * mode);

bool SetEspConnectionMode (const char * mode);  //TODO comment set mode (i.e. access point)

/*
 * Function: bool JoinNetwork (const char * ssid, const char * password)
 * --------------------
 * Pass in Access Point's SSID 
 * and password in order
 * to join the LAN.  
 *
 * returns: True if successful
 */
bool JoinNetwork (const char * ssid, const char * password);


// TODO comment
bool SendEmail (const char * msg, const char * val);



#endif
/* END OF FILE */