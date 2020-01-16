/******************************************************************************
*	FILE: disp.h
*
*	PURPOSE: Header file for display communication
*			Target device: PIC18F66K22 (advanced 8 bit MCU).   
*
*	AUTHOR: Clinton Guenther
*
*	TODO:	
*
******************************************************************************/

#ifndef __DISP_H
#define __DISP_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "struct.h"
#include "config.h"     //Project specific header file
#include "main.h"
#include "spi.h"

#define LF			1
#define CR			2

/*
 * Function: void DispInit ( void )
 * --------------------
 * Initialize the display
 *
 * returns: Nothing
 */
void DispInit ( void );

/*
 * Function: void DispCursorHome( void )
 * --------------------
 * Set the display's cursor to the home position
 *
 * returns: Nothing
 */
void DispCursorHome( void );

/*
 * Function: void DispClear( void )
 * --------------------
 * Clear the display
 *
 * returns: Nothing
 */
void DispClear( void );

/*
 * Function: void DispWriteChar (uint8_t c)
 * --------------------
 * Write a character to the display at the current
 * DDRAM memory address.  
 *
 * returns: Nothing
 */
void DispWriteChar (uint8_t c);

/*
 * Function: void DispWriteString(char * y)
 * --------------------
 * Write string to display.  Cursor position will 
 * automatically increment, however, starting position
 * shall first be defined.  Also, cursor will not
 * automatically around to line two.  
 *
 * returns: Nothing
 */
void DispWriteString(char * y);

/*
 * Function: void DispWtLnOne ( char * y )
 * --------------------
 * Write to the first line of the display.  
 * Cursor will automatically be placed at the 
 * begining of the line.  If the string is shorter
 * than 16 characters, than white spaces will be
 * written to the display until the line is
 * completely filled.  
 * This writting of white spaces prevents the need to 
 * first clear existing text on the line.     
 *
 * returns: Nothing
 */
void DispWtLnOne ( char * y );

/*
 * Function: void DispWtLnTwo ( char * y )
 * --------------------
 * Write to the second line of the display.  
 * Cursor will automatically be placed at the 
 * begining of the second line.  If the 
 * string is shorter than 16 characters, 
 * than white spaces will be written to the
 * display until the line is completely filled.  
 * This writting of white spaces prevents the need to 
 * first clear existing text on the line.     
 *
 * returns: Nothing
 */
void DispWtLnTwo ( char * y );

/*
 * Function: void DispLineTwo (void )
 * --------------------
 * A call to this function will place the 
 * cursor at the beginning of the second line.  
 *
 * returns: Nothing
 */
void DispLineTwo (void );

/*
 * Function: void DispLineOne (void )
 * --------------------
 * A call to this function will place the 
 * cursor at the beginning of the first line.  
 *
 * returns: Nothing
 */
void DispLineOne (void );

/*
 * Function: void DispSetContrast(uint8_t percentage)
 * --------------------
 * Sets the contrast of the display.  Pass in 
 * the desired contrast value (0-100%)
 *
 * returns: Nothing
 */
void DispSetContrast(uint8_t percentage);

/*
 * Function: void DispRefresh( void )
 * --------------------
 * Completely clear the dispaly and 
 * return the cursor to the home position
 * on the first line.  
 *
 * returns: Nothing
 */
void DispRefresh( void );

/*
 * Function: void DispWriteFloat (float number)
 * --------------------
 * Write a float value to the display.  
 * Value is written at wherever the cursor
 * was previously positioned.  
 *
 * returns: Nothing
 */
void DispWriteFloat (float number);

/*
 * Function: void PrintDecimalNumber (uint16_t number)
 * --------------------
 * Write a decimal value to the display.  
 * Value is written at wherever the cursor
 * was previously positioned.  
 *
 * returns: Nothing
 */
void PrintDecimalNumber (uint16_t number);

/*
 * Function: void DispWrite8b (uint8_t number)
 * --------------------
 * Write a binary eight-bit value to the display.  
 * Value is written at wherever the cursor
 * was previously positioned.  
 *
 * returns: Nothing
 */
void DispWrite8b (uint8_t number);

/*
 * Function: TODO comment
 * --------------------
 *
 * returns: Nothing
 */
void GetNewDisplayRefreshTime ( void );

/*
 * Function: TODO comment
 * --------------------
 *
 * returns: Nothing
 */
void GetNewDisplayShtdnTime( void );

#endif
/* END OF FILE */