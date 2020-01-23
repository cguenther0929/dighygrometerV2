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

struct UARTMembers  uart;
struct WifiInformation wifi;

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
    
    ResetRxBuffer();
    
    strcat(temp_string, ESP_CIPMUX_BASE_CMD);           // TODO: In the other online example, this value wasn't modified.
    strcat(temp_string, mode);
    
    PrintUARTString(temp_string,LF);
    PrintUARTBuffer();                  //TODO for debugging only

    return true;
}

void GetAssignedIpAddress ( void ) {   //TODO define a unit test that simply prints the pointer to the screen so we know this function is working as expected
    uint8_t i;
    char * temp_ptr;

    temp_ptr = wifi.assigned_ip_address;

    ResetRxBuffer();

    PrintUARTString("AT+CIFSR",LF);     //TODO need to figure out how we're going to package this IP address up

    for(i=0; i < uart.data_count; i++) {
        *temp_ptr = uart.rxbuf[uart.old_byte + i];
    }

    PrintUARTBuffer();                  //TODO for debugging only

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


// AT+CIPSTART=0,”TCP”,”152.163.0.69″,587
bool MakeTcpConnection( void ) {
    char temp_string[64]; 
    
    strcat(temp_string, "AT+CIPSTART=0");           // TODO should this be 0? 4?  
    strcat(temp_string,",");
    strcat(temp_string,"\"TCP\"");
    strcat(temp_string,",");
    strcat(temp_string, EMAIL_HOST_ADDRESS);
    strcat(temp_string, ",");
    
    PrintUARTString(temp_string, 0);            // Send the string
    
    PrintUnsignedDecimal(EMAIL_HOST_PORT, LF);  // Finish by sending the port number

    PrintUARTBuffer();                          // TODO for debugging only
    return true;
}


bool SendEmail (const char * msg, const char * val) {
    return true;

}