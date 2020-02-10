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

bool EspCommCheck ( void ) {
    
    ResetRxBuffer();
    
    PrintUARTString(ESP_AT_COMM_CHECK,LF);
    // PrintUARTString("AT\r\n",0);        //TODO this doesn't appear to work
    // PrintUARTString("AT",LF);        //TODO this doesn't appear to work
    tick20msDelay(10);    
    
    PrintUartRxBuf();                  // TODO for debugging only
    tick20msDelay(100);                 // TODO we can remove this line

    DispRefresh();
    DispWtLnOne("COM CHK DONE");
    tick20msDelay(100);     //TODO we can remove this line
    
    ResetRxBuffer();
    
    return true;
}

bool EspApOrClientMode (const char * mode) {
    // char temp_string[64]; 
    
    ResetRxBuffer();
    ResetTxBuffer();
    
    strcat(uart.txbuf, ESP_AP_OR_CLIENT_BASE_CMD);     // AT+CWMODE=
    strcat(uart.txbuf, mode);                          // ex. AT+CWMODE=1

    PrintUARTString(uart.txbuf,LF);
    tick100msDelay(1);                 // TODO we can remove this line
    
    PrintUartRxBuf();                  // TODO for debugging only
    tick100msDelay(10);                 // TODO we can remove this line
    
    ResetRxBuffer();

    return true;
}


bool SetEspCipmuxMode (const char * mode) {
    // char temp_string[64]; 
    
    ResetRxBuffer();
    ResetTxBuffer();
    
    strcat(uart.txbuf, ESP_CIPMUX_BASE_CMD);    // "AT+CIPMUX"       // TODO: In the other online example, this value wasn't modified.
    strcat(uart.txbuf, ",");           // TODO: In the other online example, this value wasn't modified.
    strcat(uart.txbuf, mode);
    
    PrintUARTString(uart.txbuf,LF);
    tick20msDelay(2);
    
    PrintUartRxBuf();                  //TODO for debugging only
    tick100msDelay(20);         // TODO longer delay so we can see what's printed
    // tick20msDelay(50);

    ResetRxBuffer();
    return true;
}

bool EspServerMode (uint8_t action, const char * port_number) {
    
    ResetRxBuffer();
    ResetTxBuffer();
    
    if(action == ACTION_OPEN_SERVER_SOCKET) {
        strcat(uart.txbuf, ESP_SERVER_BASE_CMD);            // "AT+CIPSERVER="
        strcat(uart.txbuf, ESP_OPEN_SERVER);                
        strcat(uart.txbuf, ",");                       
        strcat(uart.txbuf, port_number);
    }
    else {
        strcat(uart.txbuf, ESP_SERVER_BASE_CMD);  
        strcat(uart.txbuf, ESP_CLOSE_SERVER);     
        strcat(uart.txbuf, ",");           
        strcat(uart.txbuf, port_number);          
    }

    PrintUARTString(uart.txbuf,LF);             // Send command to WiFi module
    
    return true;

}

bool GetAssignedIpAddress ( void ) {   //TODO may need to clean up this function
    uint8_t     i = 0;
    uint8_t     match_cnt = 0;
    char        match_string[16] = {':','S','T','A','I','P',',','"','\0'};      // Match string <+CIFSR:STAIP,"NULL>
    char        * ip_addr_ptr;
    char        * buf_ptr;
    char        * match_ptr;

    ip_addr_ptr = &wifi.assigned_ip_address[0];
    buf_ptr     = &uart.rxbuf[0];
    match_ptr   = &match_string[0];

    // DispRefresh();
    // DispWtLnOne("IP ADDRESS");
    // tick100msDelay(8);
    
    ResetRxBuffer();

    PrintUARTString("AT+CIFSR",LF);         // Command to ESP8266 for retreiving IP address
    tick20msDelay(5);
    // PrintUartRxBuf();                  //TODO for debugging only

    /* 
     * Process RX Buffer until we find the words STAIP
     * Speciffically, the return will look something like
     * 
     * AT+CIFSR+CIFSR:STAIP,"192.168.0.155" \n\r  
     * AT+CIFSR+CIFSR:STAMAC,"2c:f4:32:0c:05:26" \n\r  
     * OK
     * 
     */

    while(*buf_ptr != *match_ptr) {
        buf_ptr++;
    }

    while(*buf_ptr == *match_ptr) {
        buf_ptr++;
        match_ptr++;
        match_cnt++;                    // Keep track of how many characters match
    };

    if(match_cnt < 6) {
        DispWtLnOne("Err Getting Addr");
        tick100msDelay(10);
        DispRefresh( );
        return false;
    }

    /* Should be pointing at the first character of the IP Address */
    while (*buf_ptr != '"') {
        *ip_addr_ptr = *buf_ptr;
        ip_addr_ptr++;
        buf_ptr++;
    }
    
    /* Print the trimmed IP Address */
    DispRefresh( );
    DispWtLnOne("IP Address");
    DispLineTwo( );
    DispWtLnTwo(wifi.assigned_ip_address);      // Ex print out should be 192.168.0.155
    tick100msDelay(20);
    
    ResetRxBuffer();

    return true;

}

uint8_t GetLengthIpAddress ( void ) {
    char * temp_pointer;
    uint8_t i = 0;
    
    temp_pointer = &wifi.assigned_ip_address[0];
    
    while (*temp_pointer != '\0') {
        i++;
        temp_pointer++;
    }

    /* Debug Print */
    DispRefresh( );
    DispWtLnOne("Length of IP:");
    DispLineTwo( );
    PrintDecimalNumber(i);
    tick100msDelay(20);

    return(i);
    
}

bool DisconnectWifiConnection( void ) {
    
    ResetRxBuffer( );
    
    PrintUARTString(ESP_KILL_WIFI_CONNECTION_CMD,LF);
    tick100msDelay(40);
    PrintUartRxBuf( );                          // TODO for debugging only
    tick100msDelay(10);
    return true;

}

void ResetEsp( void ) {
    PrintUARTString(ESP_RESET_MODULE_CMD,LF);
    tick100msDelay(150);
}

bool JoinNetwork (const char * ssid, const char * password) {

    ResetRxBuffer();
    ResetTxBuffer(); 
    
    strcat(uart.txbuf, ESP_JOIN_NETWORK_BASE_CMD);         // "AT+CWJAP="
    strcat(uart.txbuf, ssid);                              
    strcat(uart.txbuf, ",");
    strcat(uart.txbuf, password);

    /* Debug print */                               // TODO need to clean up this function
    // DispRefresh();
    // DispWtLnOne("Now Sending");
    // tick100msDelay(8);
    // DispRefresh( );

    // PrintUartTxBuf();
    // tick100msDelay(15);
    // DispRefresh();
    
    PrintUARTString(uart.txbuf,LF);             // Send command to WiFi module
    tick100msDelay(60);                         // This delay is imperative
    // PrintUartRxBuf();                          // TODO for debugging only
    // tick100msDelay(10);
    
    ResetRxBuffer();
    return true;
}


//TODO really need to review this function
bool SendEmail( void ) {
    
    char int_to_string[3]; 
    uint8_t i;
    uint8_t char_count;

    ResetTxBuffer(); 
    
    /* Configure SMTP connection to host */
    strcat(uart.txbuf, "AT+CIPSTART=4");           // TODO should this be AT+CIPSTART=4 or =0  
    strcat(uart.txbuf,",");
    strcat(uart.txbuf,"\"TCP\"");
    strcat(uart.txbuf,",");
    strcat(uart.txbuf, EMAIL_SMTP2GO_HOST_DOMAIN);        // Requires quotes around the domain
    strcat(uart.txbuf, ",");
    strcat(uart.txbuf, EMAIL_SMTP2GO_HOST_PORT);           // No quotes required around the number
    
    PrintUARTString(uart.txbuf, LF);    
    tick100msDelay(10);

    /* AT+CIPSEND=4,x followed by EHLO ip_address*/
    i = GetLengthIpAddress();
    i += 7;                                     // +5 for EHLO' ' and +2 for line feed characters \n\r

    char_count = sprintf(int_to_string, "%s", i);   // s tells the function we want to convert to a string

    // memset(uart.txbuf,'\0',64);
    ResetTxBuffer(); 
    strcat(uart.txbuf, "AT+CIPSEND=4,");
    strcat(uart.txbuf, int_to_string);
    PrintUARTString(uart.txbuf, LF);     
    tick100msDelay(5);
    // ClockPulseDelay(50); 

    // memset(uart.txbuf,'\0',64);
    ResetTxBuffer(); 
    strcat(uart.txbuf, "EHLO ");
    strcat(uart.txbuf, wifi.assigned_ip_address);
    PrintUARTString(uart.txbuf, LF);     
    tick100msDelay(5);
    // ClockPulseDelay(50); 

    /* Send AUTH LOGIN and configure email */
    PrintUARTString("AT+CIPSEND=4,12",LF);
    ClockPulseDelay(50); 
    PrintUARTString("AUTH LOGIN", LF);          
    tick100msDelay(5);
    // ClockPulseDelay(50); 
    
    /* Enter email username */
    // memset(uart.txbuf,'\0',64);
    ResetTxBuffer(); 
    strcat(uart.txbuf,"AT+CIPSEND=4,");
    strcat(uart.txbuf,LOGIN_USERNAME_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(uart.txbuf, LF);            
    tick100msDelay(2);

    PrintUARTString(LOGIN_EMAIL_BASE64,LF);             
    tick100msDelay(20);
    
    /* Send email password */
    // memset(uart.txbuf,'\0',64);                  //"Null" out the array -- 0xFF is forbidden
    ResetTxBuffer(); 
    strcat(uart.txbuf,"AT+CIPSEND=4,");
    strcat(uart.txbuf,LOGIN_PASSWORD_LENGTH);         // This length shall include two characters for line feed
    PrintUARTString(uart.txbuf, LF);            
    tick100msDelay(2);

    PrintUARTString(LOGIN_PASSWORD_BASE64,LF);
    tick100msDelay(20);

    /* Mail From  */
    PrintUARTString("AT+CIPSEND=4,40",LF);                      // 35 characters +2 for \n\r characters
    ClockPulseDelay(50); 
    PrintUARTString("MAIL FROM:<clinton.guenther@gmail.com>",LF);  // use your email address             
    tick100msDelay(10);
    // ClockPulseDelay(50); 

    /* Mail to */
    PrintUARTString("AT+CIPSEND=4,38",LF);                      // 36 characters +2 for \n\r characters
    ClockPulseDelay(50); 
    PrintUARTString("RCPT To:<clinton.guenther@gmail.com>",LF);
    tick100msDelay(10);
    // ClockPulseDelay(50); 

    /* Prepare to send the data */
    PrintUARTString("AT+CIPSEND=4,6",LF);
    ClockPulseDelay(50); 
    PrintUARTString("DATA",LF);
    tick100msDelay(2);

    /* Send the body of the message */
    PrintUARTString("AT+CIPSEND=4,12",LF);                      // 10 characters +2 for \n\r characters
    ClockPulseDelay(50); 
    PrintUARTString("Test Email",LF);             
    tick100msDelay(20);

    /* Send the '.' to indicate the end */
    PrintUARTString("AT+CIPSEND=4,3",LF);                       // 3 characters +2 for \n\r characters
    ClockPulseDelay(50); 
    PrintUARTString(".",LF);             
    tick100msDelay(5);

    /* Print QUIT to complete transaction */
    PrintUARTString("AT+CIPSEND=4,6",LF);             
    ClockPulseDelay(50); 
    PrintUARTString("QUIT",LF);             
    tick100msDelay(5);

    // PrintUartRxBuf();                          // TODO for debugging only
    return true;
}


bool ConfigureEmail ( void ) {
    return true;
}

