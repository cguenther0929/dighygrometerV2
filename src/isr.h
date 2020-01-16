/******************************************************************************
*   FILE: isr.h
*
*   PURPOSE: Header file for isr.c
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H

#include <xc.h>                 //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file
#include "main.h"
#include "struct.h"

/*
 * Function:  void Init_Interrupts( void )
 * --------------------
 * This function is responsible for initializing
 * interrupt features
 *
 * returns: Nothing
 */
void Init_Interrupts( void );

/*
 * Function:  __interrupt (low_priority) void main_isr( void )
 * --------------------
 * This is the low priority interrupt that will handle
 * the majority of the interrupts generated.  Examples would
 * be the UART and timer.  These are non-critical interrupts
 *
 * returns: Nothing
 */
__interrupt (low_priority) void main_isr( void );

/*
 * Function:  __interrupt (high_priority) void edges_isr( void )
 * --------------------
 * This is where the vector will point when we detect
 * and edge on one of the TACH signal inputs
 *
 * returns: Nothing
 */
__interrupt (high_priority) void edges_isr( void );

/*
 * Function:  void DisableInterrupts( void )
 * --------------------
 * Disable interrupts for time-sensitive code. Init_Interrupts
 * shall be called before calling DisableInterrupts or 
 * EnableInterrupts
 *
 * returns: Nothing
 */
void DisableInterrupts( void );

/*
 * Function:  void EnableInterrupts( void )
 * --------------------
 * Enable high and low priority interrupts.  Init_Interrupts
 * should be called before calling DisableInterrupts or 
 * EnableInterrupts
 *
 * returns: Nothing
 */
void EnableInterrupts( void );

/*
 * Function:  void EnableInterrupts( void )
 * --------------------
 * Initialize PORT B GPIO interrupts (i.e. INT0). Interrupts
 * shall first be initialized by first making a call to 
 * Init_Interrupts
 *
 * returns: Nothing
 */
void PORTBINTSetup( uint8_t channel, bool edge_rising, bool highpri );     

#endif
/* END OF FILE */