/******************************************************************************
*   FILE: dispdriver.h
*
*   PURPOSE: Header file for dispdriver.h
*
*   DEVICE: PIC18F66K22
*
*   COMPILER: Microchip XC8 v1.32
*
*   IDE: MPLAB X v3.45
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/
#ifndef __DISPDRIVER_H_
#define __DISPDRIVER_H_

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "main.h"

/********************************************************
*FUNCTION: DispSendChar(uint8_t bytein, bool command)
*PURPOSE: Send one ASCII character to the display
*PRECONDITION: Display and IO must be initialized. Note
    that this function does not clear the display, nor does
    it return the cursor prior to printing 
*POSTCONDITION: One character will be be printed.
*RETURN: Nothing
********************************************************/
void DispSendChar(uint8_t bytein, bool command);

/********************************************************
*FUNCTION: void CursorHome(void)
*PURPOSE: Return the cursor to position 0
*PRECONDITION: Display and IO must be initialized. Note that
    this function does not clear the display. 
*POSTCONDITION: Cursor will be HOME
*RETURN: Nothing
********************************************************/
void CursorHome(void);

/********************************************************
*FUNCTION: void ClearDisp(void)
*PURPOSE: Call to clear the display 
*PRECONDITION: Display and IO must be initialized. Note that
    this function does not send the cursor HOME. 
*POSTCONDITION: Display will be clear, but cursor remains
* at its previous location. 
*RETURN: Nothing
********************************************************/
void ClearDisp(void);

/********************************************************
*FUNCTION: void DisplayInit(void)
*PURPOSE: Send commads in proper sequence to initialize the 
    display
*PRECONDITION: Display IO must be initialized. 
*POSTCONDITION: Display is ready for use
*RETURN: Nothing
********************************************************/
void DisplayInit(void);

/********************************************************
*FUNCTION: void DispSendString(const char * y)
*PURPOSE: Send a string to the display.  Note, this function
    will not properly handle strings that are longer in characters 
    than what is supported.  
*PRECONDITION: Display and IO must be initialized. Note
    that this function does not clear the display, nor does
    it return the cursor prior to printing 
*POSTCONDITION: String of characters will be printed.
*RETURN: Nothing
********************************************************/
void DispSendString(const char * y);

/********************************************************
*FUNCTION: void PrintUnsignedDecimal (uint8_t number)
*PURPOSE: Print unsigned decimal value to the display 
*PRECONDITION: Display and IO must be initialized. Note
    that this function does not clear the display, nor does
    it return the cursor prior to printing 
*POSTCONDITION: Unsigned decimal number printed to display
*RETURN: Nothing
********************************************************/
void PrintUnsignedDecimal (uint8_t number);

/********************************************************
*FUNCTION: void PrintFloat (float number)
*PURPOSE: Print float value to display (%0.4f) 
*PRECONDITION: Display and IO must be initialized. Note
    that this function does not clear the display, nor does
    it return the cursor prior to printing 
*POSTCONDITION: Float value printed to display
*RETURN: Nothing
********************************************************/
void PrintFloat (float number);

/********************************************************
*FUNCTION: void DisplayON( void )
*PURPOSE: Initialize and turn on display.  This function
    will call DisplayInit()
*PRECONDITION: Display IO must be initialized.
*POSTCONDITION: Display ON and ready.
*RETURN: Nothing
********************************************************/
void DisplayON( void );

/********************************************************
*FUNCTION: void DisplayOFF( void )
*PURPOSE: Remove power from the display, and set display IO
    to HI-Z to keep from backfeeding display power through IO lins
*PRECONDITION: Display likely initialized and ON.  Proper direction
    bits set for MCU IO that controls high-side P-FET.  
*POSTCONDITION: Display OFF (low power draw)
*RETURN: Nothing
********************************************************/
void DisplayOFF( void );

#endif