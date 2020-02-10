/******************************************************************************
*   FILE: uart.h
*
*   PURPOSE:  UART header file.  Contains UART routines for the MCU.
*               Target device is a PIC16F15xx 8 bit MCU.
*
*   DEVICE: PIC18LF6620
*
*   COMPILER: Microchip XC8 v1.10
*
*   IDE: MPLAB X v1.60
*
*   TODO:  Need to modify comment style
*
*   NOTE:
*
******************************************************************************/

#ifndef __H_UART_H
#define __H_UART_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>      
#include <string.h>     // Needed mainly for memcpy function
#include "config.h"     // Project specific header file
#include "disp.h"       // Project specific header file
#include "main.h"

/* Message Buffer Sizes */
#define MAX_BUFFER              128	                        // Define max receive buffer size. 
#define MAX_ELEMENTS            MAX_BUFFER + 1  		// Number of elements that can be stored in buffer
#define UART_TMOUT_MS           30				// Value is in ms.  As a reference, a 255bit message requires ~26ms @ 9600			
#define TX_TMOUT_CNTS      	(uint16_t)(OSC_DIV4 * (UART_TMOUT_MS / 1000.0)) 	// FOSC/4 Counts.  Therefore counts = FOSC/4 * Timeout Value.  

/* UART Baud Rate Setup */         
#define BRGH_BIT                1                                       //  When one: When set to one, equation is (FOSC/(DESIRED BAUD * 16)) - 1, when zero: equation is (FOSC/(DESIRED BAUD * 64)) - 1, when zero: 
#define BRGH_BIT_IS_ONE         1                                       // Comment this out if BRGH is 0!
#define DESIRED_BAUD            115200

#ifdef  BRGH_BIT_IS_ONE
#define BRGH_VAL                (uint8_t)((MCU_OSC_FRQ / DESIRED_BAUD / 16) - 1)             //TODO put this line back in.
// #define BRGH_VAL                3

#else 
#define BRGH_VAL                (uint8_t)((MCU_OSC_FRQ / DESIRED_BAUD / 64) - 1)

#endif

extern struct UARTMembers {
        uint8_t         rxchar;                         // Most recent received character from interrupt
        char            rxbuf[MAX_BUFFER];
        char            txbuf[MAX_BUFFER];              
        uint8_t         new_byte;
        uint8_t         old_byte;
        uint8_t         data_count;
}UARTMembers;

/* CONTROL CHARACTERS */
#define LF			1
#define CR			2

#define UART_RX_HIGH_PRIORITY	1		// A one bit assigned to RCxIP indicates the receiver shall be treated with high priority
#define UART_RX_LO_PRIORITY		0		

/*
 * Function: void EnableUart1Interrupts( uint8_t RECEIVE_PRIORITY )
 * --------------------
 * TODO need to comment
 *
 * returns: Nothing 
 */
void EnableUart1Interrupts( uint8_t RECEIVE_PRIORITY );


/********************************************************
*FUNCTION: void SetUpEUSART( void )
*PURPOSE: Set up the EUSART
*PRECONDITION: EUSART is not setup
*POSTCONDITION: EUSART is now ready to use
*RETURN: Nothing
********************************************************/
void SetUpUART( void );

/********************************************************
*FUNCTION: void PrintUARTString (const char * y, uint8_t action )
*PURPOSE: Send a string out the UART
*PRECONDITION: None
*POSTCONDITION: String now sent out the UART
*RETURN: Nothing
********************************************************/
void PrintUARTString (const char *y, uint8_t action );

/*
 * Function: void PrintUartRxBuf ( void )
 * --------------------
 * Print the contents of the UART 
 * RX buffer to the display
 *
 * returns: Nothing 
 */
void PrintUartRxBuf ( void );

void PrintUartTxBuf ( void );           //TODO comment

/*
 * Function: void ResetRxBuffer( void )
 * --------------------
 * Reset UART receive buffer by
 * way of the built in memset function 
 *
 * returns: Nothing 
 */
void ResetRxBuffer( void );

/*
 * Function: void ResetTxBuffer( void )
 * --------------------
 * Reset UART transmit buffer by
 * way of the built in memset function 
 *
 * returns: Nothing 
 */
void ResetTxBuffer( void );

// /********************************************************
// *FUNCTION: int IntegerInput(const char * y, uint8_t action)
// *PURPOSE: User enters a string of ascii characters and theseReceive
// 		are then converted to an integer number. 
// *PRECONDITION: Uart must be initialized along with 
// 				uart RX interrupts.  
// *POSTCONDITION: User allowed to enter a number 
// *RETURN: Integer value of ascii string entered.  
// ********************************************************/
// int IntegerInput(const char * y, uint8_t action);

// //TODO: Need to comment
// uint8_t BinaryInput(const char * y, uint8_t action);

// /********************************************************
// *FUNCTION: BYTE GetNumberByte( void )
// *PURPOSE: Recieve numerical input from user (value 0 to 255).
// 			Values larger than 255 will not be accepted.
// *PRECONDITION: Uart must be initialized along with 
// 				uart RX interrupts.  
// *POSTCONDITION: User ascii number uint8_t is converted to a number.
// *RETURN: Number that user entered (BYTE is max size)
// ********************************************************/
// BYTE GetNumberByte( void );

/********************************************************
*FUNCTION: void TXmessage(const char * y)
*PURPOSE: Send message string to PTE PC. Specially developed
        for the PTE project, thus this function will not be
        relevant for any other projects.  
*PRECONDITION: UART must be configured
*POSTCONDITION: Message sent to PTE PC using defined protocol
*RETURN: Nothing
********************************************************/
void TXmessage(const char *y);

/********************************************************
*FUNCTION: void PrintUnsignedDecimal (uint16_t number, uint8_t action)
*PURPOSE: Print an unsigned decimal value
*PRECONDITION: None
*POSTCONDITION: Unsigned decimal value should be printed
*RETURN: Nothing
********************************************************/
void PrintUnsignedDecimal (uint16_t number, uint8_t action);

// /********************************************************
// *FUNCTION: void UARTRead( void )
// *PURPOSE: Read the BYTE stored in the UART buffer
// *PRECONDITION: None
// *POSTCONDITION: BYTE now read from the UART buffer
// *RETURN: Nothing
// ********************************************************/
// void UARTRead( void );


// /********************************************************
// *FUNCTION: void PrintLargeDecimal (DWORD number, uint8_t action)
// *PURPOSE: Print large number (up to 8 digits)
// *PRECONDITION: UART must be configured
// *POSTCONDITION: Unsigned decimal value should be printed
// *RETURN: Nothing
// ********************************************************/
// void PrintLargeDecimal (DWORD number, uint8_t action);

// /********************************************************
// *FUNCTION: void PrintBinInt (uint16_t number, uint8_t action)
// *PURPOSE: Print a binary integer to the console (16 bits)
// *PRECONDITION: None
// *POSTCONDITION: Binary integer number printed to the console
// *RETURN: Nothing
// ********************************************************/
// void PrintBinInt (uint16_t number, uint8_t action);

// /********************************************************
// *FUNCTION: void PrintBinChar(uint16_t number, uint8_t action)
// *PURPOSE: Print a binary number to the console (8 bits)
// *PRECONDITION: None
// *POSTCONDITION: Binary number now printed to console
// *RETURN: Nothing
// ********************************************************/
// void PrintBinChar(uint16_t number, uint8_t action);

// /********************************************************
// *FUNCTION: void ClearCursorUp( void )
// *PURPOSE: To clear the screen from the cursor up
// *PRECONDITION: None
// *POSTCONDITION: Screen now cleared from the cursor on up
// *RETURN: Nothing
// ********************************************************/
// void ClearCursorUp( void );

// /********************************************************
// *FUNCTION: void ClearScreen( void )
// *PURPOSE: To clear the console
// *PRECONDITION: None
// *POSTCONDITION: Screen is now cleared
// *RETURN: Nothing
// ********************************************************/
// void ClearScreen( void );

// /********************************************************
// *FUNCTION: void CursorTopLeft( void )
// *PURPOSE: To place the cursor at the top left of the console
// *PRECONDITION: None
// *POSTCONDITION: Cursor now at the top left of the screen
// *RETURN: Nothing
// ********************************************************/
// void CursorTopLeft( void );

// /********************************************************
// *FUNCTION: ResetTerminal( void )
// *PURPOSE: To reset the terminal.  This will clear everything
// *PRECONDITION: None
// *POSTCONDITION: The screen is not reset and completely cleared
// *RETURN: Nothing
// ********************************************************/
// void ResetTerminal( void );

// /********************************************************
// *FUNCTION: void PrintFloat (float number, uint8_t action)
// *PURPOSE: Print a float number to the console
// *PRECONDITION: None
// *POSTCONDITION: Float number now printed to the console
// *RETURN: Nothing
// ********************************************************/
// void PrintFloat (float number, uint8_t action);

// /********************************************************
// *FUNCTION: void ClearLine( void )
// *PURPOSE: Clear the current line that the cursor is in
// *PRECONDITION: Line may be filled with characters
// *POSTCONDITION: Line that cursor is on is now cleared
// *RETURN: Nothing
// ********************************************************/
// void ClearLine( void );

/********************************************************
*FUNCTION: void PrintChar( uint8_t inchar, uint8_t action )
*PURPOSE: Print a single character to the console
*PRECONDITION: None
*POSTCONDITION: Single character now printed to the console
*RETURN: Nothing
********************************************************/
void PrintChar( uint8_t inchar, uint8_t action );

/********************************************************
*FUNCTION: void PrintLineSpaces( BYTE spaces )
*PURPOSE: In terminal, insert line feeds. Qty defined by 
*			spaces.
*PRECONDITION: None
*POSTCONDITION: Line feeds will be inserted.  
*RETURN: Nothing
********************************************************/
void PrintLineSpaces( uint8_t spaces );

/********************************************************
*FUNCTION: void TXWait( void )
*PURPOSE: Wait for the transmit buffer to empty or timeout 
        which ever happens first.  
*PRECONDITION: UART shall be setup.
*POSTCONDITION: Transmit buffer should be empty
*RETURN: Nothing
********************************************************/
void TXWait( void );

#endif
/* END OF FILE */