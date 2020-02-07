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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "main.h"
#include "disp.h"

extern struct WifiInformation         //Structure to pass information that is shared among modules
{
    /* Display Related  */
    char    assigned_ip_address[16];        // Array to store IP address.  Max characters is 16 ( XXX.XXX.XXX.XXX + NULL Terminator)
    
} WifiInformation;



#define LF			            1
#define CR			            2

/*
 * Base64 coded format of clinton.debug@gmail.com
 * Use base64encode.org to create
 * Length of string is 32 characters, but total length
 * is 34 characters when accounting for line feed
 */
#define LOGIN_EMAIL_BASE64                  "Y2xpbnRvbi5kZWJ1Z0BnbWFpbC5jb20="
#define LOGIN_USERNAME_LENGTH               "34"

/*
 * Base64 coded format of email password
 * for account clinton.debug@gmail.com
 * Length of string is 20 characters, but total length
 * is 22 characters when accounting for line feed
 */
#define LOGIN_PASSWORD_BASE64               "TUdINTY3JiooNDI0MjYy"
#define LOGIN_PASSWORD_LENGTH               "22"

#define ESP_AT_COMM_CHECK                   "AT"

#define ESP_MAKE_TCP_CONN_BASE_CMD          "AT+CIPSTART=0"
#define EMAIL_GMAIL_HOST_DOMAIN             "\"smtp.gmail.com\""
#define EMAIL_GMAIL_HOST_PORT               "465"

#define EMAIL_SMTP2GO_HOST_DOMAIN           "\"not.defined\""
#define EMAIL_SMTP2GO_HOST_PORT             "2525"

#define ESP_JOIN_NETWORK_BASE_CMD           "AT+CWJAP="
#define WIFI_ROUTER_SSID                    "\"CJG_GbE_2G4\""
#define WIFI_ROUTER_PASSWORD                "\"GlockHK23\""

#define ESP_CIPMUX_BASE_CMD                 "AT+CIPMUX="
#define ESP_CIPMUX_SINGLE_CONNECTION        "0"         
#define ESP_CIPMUX_MULTIPLE_CONNECTION      "1"

#define ESP_AP_OR_CLIENT_BASE_CMD           "AT+CWMODE="
#define ESP_CLIENT_MODE                     "1"
#define ESP_AP_MODE                         "2"
#define ESP_AP_AND_CLIENT_MODE              "3"

#define ESP_KILL_WIFI_CONNECTION_CMD        "AT+CWQAP"

#define ESP_RESET_MODULE_CMD                "AT+RST"

#define ESP_SERVER_BASE_CMD                 "AT+CIPSERVER="
#define ESP_CLOSE_SERVER                    "0"
#define ESP_OPEN_SERVER                     "1"
#define ACTION_CLOSE_SERVER_SOCKET          0
#define ACTION_OPEN_SERVER_SOCKET           1



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

bool EspApOrClientMode (const char * mode);  //TODO comment set mode (i.e. access point)

bool DisconnectWifiConnection( void );

void ResetEsp( void ); //TODO comment

bool EspServerMode (uint8_t action, const char * port_number);  //TODO comments

bool EspCommCheck ( void ); //TODO comment

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



bool SendEmail( void ); //TODO comment

void GetAssignedIpAddress ( void );

uint8_t GetLengthIpAddress ( void ); //Get the length of the ip address.  192.168.0.1 = 11

bool ConfigureEmail ( void );  //TODO comment   


#endif
/* END OF FILE */