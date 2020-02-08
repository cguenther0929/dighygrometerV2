/******************************************************************************
*   FILE: uart.c
*
*   PURPOSE: UART main file.  Routines contained in this file allow the MCU
*           to send data over the UART.  
*
*   DEVICE: PIC18LF6620
*
*   COMPILER: Microchip XC8 v1.10
*
*   IDE: MPLAB X v1.60
*
*   TODO:
*
*   NOTE:
*
******************************************************************************/
#include "uart.h"

struct UARTMembers uart ;            //Structure for handling UART RXed data

void EnableUart1Interrupts( uint8_t RECEIVE_PRIORITY ) {
    RC1IE = 1;          // Enable the UART receive interrupt -- flag cleared when buffer is read
    RECEIVE_PRIORITY = UART_RX_HIGH_PRIORITY ? (RC1IP = UART_RX_HIGH_PRIORITY):(RC1IP = UART_RX_LO_PRIORITY);
}    


void SetUpUART( void ) {

    /* RCSTA (RECEIVE STATUS AND CONTROL REGISTER) SETTINGS */
    SPEN1 = 1;              //This will enable the serial port pins see datasheet 199/380
    TRISC7 = input;         //Make the MCU RXD pin an input -- see datasheet 199/380
    TRISC6 = output;        //Make the MCU TXD pin an output -- see datasheet 199/380

    /* TXSTA CONTROL REGISTER SETTINGS -- SEE PAGE 200/380 */
    SYNC1 = 0;              // Set the module up for asynchronous communication
    CSRC1 = 1;              // Clock generated internally from BAUD Rate Generator
    TXEN1 = 1;              // Enable the transmitter
    BRGH1 = BRGH_BIT;       

    /* RCSA CONTROL REGISTER SETTINGS -- SEE PAGE 201/380 */
    SPEN1 = 1;              // Map port pins to the USART module
    CREN1 = 1;              // This enables the receiver

    /* SET UP THE BAUD RATE GENERATOR -- FORMULA ON PAGE 202/380 */
    SPBRG1 = BRGH_VAL;      

}


void PrintChar( uint8_t inchar, uint8_t action ) {
    uint16_t i;                     //Use this as a general counter
    TXREG1 = inchar;                    //Load the U1 TX buffer with the current character
    while (TRMT1 == 0);

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
            TXREG1 = '\r';      //Return the cursor
            TXWait();
            TXREG1 = '\n';      //Put us on a new line -- must be in this order...
            TXWait();
    }
    else if(action == CR) {
        TXREG1 = '\r';      //Return the cursor
        TXWait();
    }
}

void PrintUARTBuffer ( void ){
    uint8_t char_count = 0;

    DispRefresh();
    while(uart.data_count > 0 ){
        if(uart.rxbuf[uart.old_byte] != 0x0a || uart.rxbuf[uart.old_byte] != 0x0d) {

            DispWriteChar(uart.rxbuf[uart.old_byte]);
            (uart.old_byte >= MAX_BUFFER) ? (uart.old_byte = 0):(uart.old_byte++);
            uart.data_count--;
            char_count++;
        }
        else {
            (uart.old_byte >= MAX_BUFFER) ? (uart.old_byte = 0):(uart.old_byte++);
            uart.data_count--;
            char_count++;
        }
        
        if(char_count >= 16) {
            DispLineTwo ();
        }
        
        if(char_count >= 32) {
            char_count = 0;
            tick100msDelay(8);
            DispRefresh();
            // DispWtLnOne("OVERFLOW 0001");        //TODO remove these lines? 
            // break;
        }

    }

}

void PrintUARTString (const char * y, uint8_t action ) {

    while(*y != '\0'){
        TXREG1 = *y;                    //Load the U1 TX buffer with the current character
        TXWait();
        y++;                           //Increment the pointer memory address
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        TXREG1 = '\r';      //Return the cursor
        TXWait();
        TXREG1 = '\n';          //Put us on a new line -- must be in this order...
        TXWait();
    }
    else if(action == CR) {
        TXREG1 = '\r';      //Return the cursor
        TXWait();
    }
    return;
}

void TXmessage(const char *y){

    while(*y != 0xFF)
    {
        TXREG1 = *y;                    //Load the U1 TX buffer with the current character
        TXWait();
        y++;                           //Increment the pointer memory address
    }
    return;
}

void PrintLineSpaces( uint8_t spaces ) {
    uint16_t i = 0;         //Use this as a counter
	for(i=0;i<spaces;i++) {
		TXREG1 = '\r';      // Return the cursor
		TXWait();
		TXREG1 = '\n';      // Put us on a new line - - Must be in this order...
		TXWait();
	}
}

void TXWait( void ) {
    uint16_t i = 0;       //Use for timer
    while(TRMT1 == 0 && i < TX_TMOUT_CNTS){
        i++;
    }
    if(i >= TX_TMOUT_CNTS) {
        DispWtLnOne("UART Error.");
    }

    return;
}

void ResetRxBuffer( void ) {
    RC1IE = 0;                                                      //Turn off UART interrupts while resetting buffer
    uart.old_byte = uart.new_byte = 0;                              //Reset the pointers
    uart.data_count = 0;                                            //Reset the data counter
    memset(uart.rxbuf,'\0',MAX_ELEMENTS);                           //Null out the buffer
    RC1IE = 1;                                                      //Turn UART interrupts back on
}

void PrintUnsignedDecimal (uint16_t number, uint8_t action) {
    char temphex[5];            // Define the array that will hold the ASCII values
    uint8_t i;                  // Use this as a counter
    uint16_t j=0;
    uint8_t decimal_count;      // This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = sprintf(temphex, "%u", number); //u tells the function we want an unsigned decimal number

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        TXREG1 = (temphex[i]);
        TXWait();
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        TXREG1 = '\r';      //Return the cursor
        TXWait();
        TXREG1 = '\n';      //Put us on a new line -- must be in this order...
        TXWait();
    }
        else if(action == CR) {
            TXREG1 = '\r';      //Return cursor only
            TXWait();
        }
}

// void PrintLargeDecimal(DWORD number, uint8_t action){
//     char temphex[8];        //Define the array that will hold the ASCII values
//     uint8_t i;                //Use this as a counter
//     uint16_t j;
//     uint8_t decimal_count;    //This is how many digits are written

//     /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
//     decimal_count = sprintf(temphex, "%u", number); //u tells the function we want an unsigned decimal number

//     for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
//         TXREG1 = (temphex[i]);
//         TXWait();
//     }

//     /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//         TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//         TXWait();
//     }
//         else if(action == CR) {
//             TXREG1 = '\r';      //Return cursor only
//             TXWait();
//         }
// }

// void PrintFloat (float number, uint8_t action) {
//     char temphex[8];        //Define the array that will hold the ASCII values
//     uint8_t i;                //Use this as a counter
//     uint16_t j;
//     uint8_t decimal_count; //This is how many digits are written

//     /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
//     decimal_count = sprintf(temphex, "%.4f", number);   //f tells the function we want to print a float value

//     for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
//         TXREG1 = (temphex[i]);
//         TXWait();
//     }

//     /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//         TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//         TXWait();
//     }
//     else if(action == CR) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//     }
// }

// uint8_t BinaryInput(const char * y, uint8_t action) {
// 	uint8_t tempbuf[8];						//Buffer input from user
// 	memset(tempbuf,0x00,8);
// 	BYTE i=0;
// 	BYTE j=0;									//Use as a coutner
// 	BYTE poweroftwo=0;
// 	uint8_t outputnumber=0;
// 	BYTE bits=0;
// 	PrintUARTString(y,0);					//Print out the directive string
	
// 	uart.rxchar = '?';
// 	while(uart.rxchar != 13 && i<8){				
// 		uart.rxchar = '?';
// 		while(uart.rxchar == '?');			//Wait for character to be entered
// 		PrintChar(uart.rxchar,0);		//Confirm user input
// 		if(uart.rxchar != 13){
// 			uart.rxchar -= 0x30;			//Convert from ascii
// 			tempbuf[i] = uart.rxchar;
// 			i++;							//Increment buffer location
// 		}
// 	}
// 	bits = i-1;
		
// 	for(j=0;j < i;j++){					//Calculate the decimal number entered
// 		poweroftwo = (BYTE)(0x01 << j);
// 		outputnumber = (outputnumber + ((tempbuf[bits]) * (0x01 << j)));
// 		bits--;
// 	}	
	
// 	/* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//             TXREG1 = '\r';      //Return the cursor
//             TXWait();
//             TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//             TXWait();
//     }
//     else if(action == CR) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//     }
	
// 	return outputnumber;
// }


// int IntegerInput(const char * y, uint8_t action){
// 	uint8_t tempbuf[5];						//Buffer input from user
// 	memset(tempbuf,0x00,5);
// 	BYTE i=0;									//Use as a coutner
// 	int	outputnumber;
	
// 	PrintUARTString(y,0);					//Print out the directive string
	
// 	uart.rxchar = '?';
// 	while(uart.rxchar != 13 && i<=3){				
// 		uart.rxchar = '?';
// 		while(uart.rxchar == '?');			//Wait for character to be entered
// 		PrintChar(uart.rxchar,0);		//Confirm user input
// 		if(uart.rxchar != 13){
// 			tempbuf[i] = uart.rxchar;
// 			i++;							//Increment buffer location
// 		}
// 	}
	
// 	/* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//             TXREG1 = '\r';      //Return the cursor
//             TXWait();
//             TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//             TXWait();
//     }
//     else if(action == CR) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//     }
	
// 	outputnumber = atoi(tempbuf);			//Get integer representation
	
// 	return outputnumber;
// }

// void PrintBinInt(uint16_t number, uint8_t action) {
//     uint16_t temp;      //Use for temporary value
//     uint16_t i;         //Counter value
//     uint16_t j;         //Counter value
//     for(i = 16; i > 0 ; i--) {
//         temp = number >> (i - 1);
//         temp = temp & 0x01;
//         temp = temp + 0x30;         //Turn this into an ASCII value
//         TXREG1 = temp;
//         TXWait();
//     }

//     /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//         TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//         TXWait();
//     }
//     else if(action == CR) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//     }
//     return;

// }

// void PrintBinChar(uint16_t number, uint8_t action) {
//     uint16_t temp;      //Use for temporary value
//     uint16_t i;         //Counter value
//     uint16_t j=0;         //Counter value
//     for(i = 8; i > 0; i--) {
//         temp = number >> (i - 1);
//         temp = temp & 0x01;
//         temp = temp + 0x30;         //Turn this into an ASCII value
//         TXREG1 = temp;
//         TXWait();
//     }

//     /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
//     if(action == LF) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//         TXREG1 = '\n';      //Put us on a new line -- must be in this order...
//         TXWait();
//     }
//     else if(action == CR) {
//         TXREG1 = '\r';      //Return the cursor
//         TXWait();
//     }
// }


// void ClearCursorUp( void ) {
//     uint16_t i;             //Use this as a counter

//     TXREG1 = 0x1B;                  //Send the VT100 ESC character
//     TXWait();
//     PrintUARTString("[1J",0);        //Send the rest of the sequence to clear the screen

// }

// void ClearScreen( void ) {
//     uint16_t i;             //Use this as a counter

//     TXREG1 = 0x1B;                  //Send the VT100 ESC character
//     TXWait();
//     PrintUARTString("[2J",0);        //Send the rest of the sequence to clear the screen

// }

// void ClearLine( void ) {
//     uint16_t i;             //Use this as a counter
//     TXREG1 = 0x1B;                  //Send the VT100 ESC character
//     TXWait();
//     PrintUARTString("[2K",0);        //Send the rest of the sequence to clear the screen

// }

// void CursorTopLeft( void ) {
//     uint16_t i;             //Use this as a counter

//     TXREG1 = 0x1B;                  //Send the VT100 ESC character
//     TXWait();
//     PrintUARTString("[H",0);     //Send the rest of the sequence to clear the screen
// }

// void ResetTerminal( void ) {
//     uint16_t i;             //Use this as a counter

//     TXREG1 = 0x1B;                  //Send the VT100 ESC character
//     TXWait();
//     PrintUARTString("c",0);      //Send the rest of the sequence to clear the screen
// }

