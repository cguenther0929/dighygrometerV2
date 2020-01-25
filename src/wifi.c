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

    temp_ptr = & wifi.assigned_ip_address[0];

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


//TODO really need to review this function
bool MakeSmtpConnection( void ) {
    char temp_string[64];
    char int_to_string[3]; 
    uint8_t i;
    uint8_t char_count;

    /* Configure SMTP connection to host */
    strcat(temp_string, "AT+CIPSTART=4");           // TODO should this be 0? 4?  
    strcat(temp_string,",");
    strcat(temp_string,"\"TCP\"");
    strcat(temp_string,",");
    strcat(temp_string, EMAIL_GMAIL_HOST_DOMAIN);        // Requires quotes around the domain
    strcat(temp_string, ",");
    strcat(temp_string, EMAIL_GMAIL_HOST_PORT);           // No quotes required around the number
    
    PrintUARTString(temp_string, LF);            

    /* AT+CIPSEND=4,x followed by EHLO ip_address*/
    i = GetLengthIpAddress();
    i += 5;                                     // Add five characters for account for EHLO command  //TODO we need to make sure this is accurate

    char_count = sprintf(int_to_string, "%s", i);   // s tells the function we want to convert to a string

    memset(temp_string,'\0',64);
    strcat(temp_string, "AT+CIPSEND=4,");
    strcat(temp_string, int_to_string);
    PrintUARTString(temp_string, LF);     

    memset(temp_string,'\0',64);
    strcat(temp_string, "EHLO ");
    strcat(temp_string, wifi.assigned_ip_address);
    PrintUARTString(temp_string, LF);     

    /* Send AUTH LOGIN and configure email */
    PrintUARTString("AT+CIPSEND=4,12",LF);
    PrintUARTString("AUTH LOGIN", LF);          
    
    /* Enter email username */
    memset(temp_string,'\0',64);
    strcat(temp_string,"AT+CIPSEND=4,");
    strcat(temp_string,LOGIN_USERNAME_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(temp_string, LF);            

    PrintUARTString(LOGIN_EMAIL_BASE64,LF);             
    
    /* Send email password */
    memset(temp_string,'\0',64);                  //"Null" out the array -- 0xFF is forbidden
    strcat(temp_string,"AT+CIPSEND=4,");
    strcat(temp_string,LOGIN_PASSWORD_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(temp_string, LF);            

    PrintUARTString(LOGIN_PASSWORD_BASE64,LF);

    /* Mail From  */
    PrintUARTString("AT+CIPSEND=4,37",LF);
    tick20msDelay(2);
    PrintUARTString("MAIL FROM:<clinton.debug@gmail.com>",LF);  // use your email address             

    /* Mail to */
    PrintUARTString("AT+CIPSEND=4,38",LF);
    tick20msDelay(2);
    PrintUARTString("RCPT To:<clinton.guenther@gmail.com>",LF);

    /* Prepare to send the data */
    PrintUARTString("AT+CIPSEND=4,6",LF);
    tick20msDelay(2);
    PrintUARTString("DATA",LF);

    /* Send the body of the message */
    PrintUARTString("AT+CIPSEND=4,12",LF);
    tick20msDelay(2);
    PrintUARTString("Test Email",LF);             

    /* Send the '.' to indicate the end */
    PrintUARTString("AT+CIPSEND=4,3",LF);             
    PrintUARTString(".",LF);             

    /* Print QUIT to complete transaction */
    PrintUARTString("AT+CIPSEND=4,6",LF);             
    PrintUARTString("QUIT",LF);             

    // PrintUARTBuffer();                          // TODO for debugging only
    return true;
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

bool ConfigureEmail ( void ) {

    return true;
}


bool SendEmail (const char * msg, const char * val) {
    return true;

}