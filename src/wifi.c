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

bool EspApOrClientMode (const char * mode) {
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
    strcat(temp_string, ",");           // TODO: In the other online example, this value wasn't modified.
    strcat(temp_string, mode);
    
    PrintUARTString(temp_string,LF);
    PrintUARTBuffer();                  //TODO for debugging only

    return true;
}

bool EspServerMode (uint8_t action, const char * port_number) {
    char temp_string[64]; 
    ResetRxBuffer();
    if(action == ACTION_OPEN_SERVER_SOCKET) {
        strcat(temp_string, ESP_SERVER_BASE_CMD);       
        strcat(temp_string, ESP_OPEN_SERVER);           
        strcat(temp_string, ",");                       
        strcat(temp_string, port_number);
    }
    else {
        strcat(temp_string, ESP_SERVER_BASE_CMD);  
        strcat(temp_string, ESP_CLOSE_SERVER);     
        strcat(temp_string, ",");           
        strcat(temp_string, port_number);          
    }

    PrintUARTString(temp_string,LF);
    
    return true;

}

void GetAssignedIpAddress ( void ) {   
    uint8_t i;
    char * temp_ptr;

    temp_ptr = & wifi.assigned_ip_address[0];

    ResetRxBuffer();

    PrintUARTString("AT+CIFSR",LF);         // Command to ESP8266 for retreiving IP address

    for(i=0; i < uart.data_count; i++) {
        if((uart.rxbuf[uart.old_byte + i] != 0x0A) && (uart.rxbuf[uart.old_byte + i] != 0x0D))   // Do not collect line feed characters
            *temp_ptr = uart.rxbuf[uart.old_byte + i];
    }

    PrintUARTBuffer();                  //TODO for debugging only
    ResetRxBuffer();

}

uint8_t GetLengthIpAddress ( void ) {
    char * temp_pointer;
    uint8_t i = 0;
    
    temp_pointer = & wifi.assigned_ip_address[0];
    
    while (*temp_pointer != '\0') {
        i++;
        temp_pointer++;
    }

    return(i);
    
}

bool DisconnectWifiConnection( void ) {
    
    PrintUARTString(ESP_KILL_WIFI_CONNECTION_CMD,LF);
    return true;

}

void ResetEsp( void ) {
    PrintUARTString(ESP_RESET_MODULE_CMD,LF);
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


//TODO really need to review this function
bool SendEmail( void ) {
    char temp_string[64];
    char int_to_string[3]; 
    uint8_t i;
    uint8_t char_count;

    /* Configure SMTP connection to host */
    strcat(temp_string, "AT+CIPSTART=4");           // TODO should this be AT+CIPSTART=4 or =0  
    strcat(temp_string,",");
    strcat(temp_string,"\"TCP\"");
    strcat(temp_string,",");
    strcat(temp_string, EMAIL_GMAIL_HOST_DOMAIN);        // Requires quotes around the domain
    strcat(temp_string, ",");
    strcat(temp_string, EMAIL_GMAIL_HOST_PORT);           // No quotes required around the number
    
    PrintUARTString(temp_string, LF);    
    ClockPulseDelay(50); 

    /* AT+CIPSEND=4,x followed by EHLO ip_address*/
    i = GetLengthIpAddress();
    i += 5;                                     // Add five characters for account for EHLO command  //TODO we need to make sure this is accurate

    char_count = sprintf(int_to_string, "%s", i);   // s tells the function we want to convert to a string

    memset(temp_string,'\0',64);
    strcat(temp_string, "AT+CIPSEND=4,");
    strcat(temp_string, int_to_string);
    PrintUARTString(temp_string, LF);     
    ClockPulseDelay(50); 

    memset(temp_string,'\0',64);
    strcat(temp_string, "EHLO ");
    strcat(temp_string, wifi.assigned_ip_address);
    PrintUARTString(temp_string, LF);     
    ClockPulseDelay(50); 

    /* Send AUTH LOGIN and configure email */
    PrintUARTString("AT+CIPSEND=4,12",LF);
    ClockPulseDelay(50); 
    PrintUARTString("AUTH LOGIN", LF);          
    ClockPulseDelay(50); 
    
    /* Enter email username */
    memset(temp_string,'\0',64);
    strcat(temp_string,"AT+CIPSEND=4,");
    strcat(temp_string,LOGIN_USERNAME_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(temp_string, LF);            
    ClockPulseDelay(50); 

    PrintUARTString(LOGIN_EMAIL_BASE64,LF);             
    ClockPulseDelay(50); 
    
    /* Send email password */
    memset(temp_string,'\0',64);                  //"Null" out the array -- 0xFF is forbidden
    strcat(temp_string,"AT+CIPSEND=4,");
    strcat(temp_string,LOGIN_PASSWORD_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(temp_string, LF);            
    ClockPulseDelay(50); 

    PrintUARTString(LOGIN_PASSWORD_BASE64,LF);
    ClockPulseDelay(50); 

    /* Mail From  */
    PrintUARTString("AT+CIPSEND=4,37",LF);
    ClockPulseDelay(50); 
    PrintUARTString("MAIL FROM:<clinton.debug@gmail.com>",LF);  // use your email address             
    ClockPulseDelay(50); 

    /* Mail to */
    PrintUARTString("AT+CIPSEND=4,38",LF);
    ClockPulseDelay(50); 
    PrintUARTString("RCPT To:<clinton.guenther@gmail.com>",LF);
    ClockPulseDelay(50); 

    /* Prepare to send the data */
    PrintUARTString("AT+CIPSEND=4,6",LF);
    ClockPulseDelay(50); 
    PrintUARTString("DATA",LF);
    ClockPulseDelay(50); 

    /* Send the body of the message */
    PrintUARTString("AT+CIPSEND=4,12",LF);
    ClockPulseDelay(50); 
    PrintUARTString("Test Email",LF);             
    ClockPulseDelay(50); 

    /* Send the '.' to indicate the end */
    PrintUARTString("AT+CIPSEND=4,3",LF);             
    ClockPulseDelay(50); 
    PrintUARTString(".",LF);             
    ClockPulseDelay(50); 

    /* Print QUIT to complete transaction */
    PrintUARTString("AT+CIPSEND=4,6",LF);             
    ClockPulseDelay(50); 
    PrintUARTString("QUIT",LF);             
    ClockPulseDelay(50); 

    // PrintUARTBuffer();                          // TODO for debugging only
    return true;
}


bool ConfigureEmail ( void ) {
    return true;
}

