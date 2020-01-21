/******************************************************************************
*   FILE: wifi.c
*
*   PURPOSE: WiFi source file
*
*   TODO: Need to 'harden' certain functions opposed to just returning true
*
*   NOTE: 
*
******************************************************************************/
#include "wifi.h"

bool SetEspConnectionMode (const char * mode) {
    char temp_string[64]; 
    
    strcat(temp_string, ESP_AP_OR_CLIENT_BASE_CMD);
    strcat(temp_string, mode);

    PrintUARTString(temp_string,LF);
    
    PrintUARTBuffer();                  //TODO for debugging only

    return true;
}


bool SetEspCipmuxMode (const char * mode) {
    char temp_string[64]; 
    
    strcat(temp_string, ESP_CIPMUX_BASE_CMD);
    strcat(temp_string, mode);
    
    PrintUARTString(temp_string,LF);
    PrintUARTBuffer();                  //TODO for debugging only

    return true;
}


bool JoinNetwork (const char * ssid, const char * password) {
    char temp_string[64]; 
    
    strcat(temp_string, ESP_JOIN_NETWORK_BASE_CMD);
    strcat(temp_string, ssid);
    strcat(temp_string, ",");
    strcat(temp_string, password);
    
    
    PrintUARTString(temp_string,LF);
    PrintUARTBuffer();                  //TODO for debugging only

    return true;
}

bool SendEmail (const char * msg, const char * val) {
    return true;

}