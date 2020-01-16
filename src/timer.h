/******************************************************************************
*   FILE: timer.h
*
*   PURPOSE: Header file for timer.c
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include <xc.h>                 //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file

/* TIMER MNEMONICS */
#define     TMR0_FSOC_DIV_BY_4  0
#define     TMR0_PIN_T0CKI      1

/*
 * Function: void Timer0Init(uint8_t interrupts, uint16_t prescaler, uint8_t clksource )
 * --------------------
 * Timer 0 is initialized 
 * Note: function designed such that timer will operate in 16 bit mode.
 *
 * returns: Nothing
 */
void Timer0Init(uint8_t interrupts, uint16_t prescaler, uint8_t clksource );

/*
 * Function: void Timer0On( void )
 * --------------------
 * Timer0 will begin to increment
 *
 * returns: Nothing
 */
void Timer0On( void );

/*
 * Function: void Timer1Init( uint8_t interrupts, uint8_t prescaler, uint8_t clksource )
 * --------------------
 * Timer 1 is initialized When reading from timer one 
 * in 16bit mode, we must read from the LOW byte first. 
 * When we write to the timer, we must write to the LOW byte
 * and then to the HIGH byte.  
 *
 * returns: Nothing
 */
void Timer1Init( uint8_t interrupts, uint8_t prescaler, uint8_t clksource );

/*
 * Function: void Timer1On(uint8_t RegHigh, uint8_t RegLow)
 * --------------------
 * Timer1 will begin to increment.  16 bit register shall
 * be passed in.
 *
 * returns: Nothing
 */
void Timer1On(uint8_t RegHigh, uint8_t RegLow);

/*
 * Function: void Timer1Off( void )
 * --------------------
 * Timer1 will cease to increment.  
 *
 * returns: Nothing
 */
void Timer1Off( void );

/*
 * Function: void Timer2Init(bool interrupts, uint8_t prescaler, uint8_t postscaler)
 * --------------------
 * Timer 2 is initialized.
 *
 * returns: Nothing
 */
void Timer2Init(uint8_t interrupts, uint8_t prescaler, uint8_t postscaler);     

/*
 * Function: void Timer2On(uint8_t period)
 * --------------------
 * Timer1 will begin to increment.  8 bit value to
 * load into the timer's register shall be passed in.
 *
 * returns: Nothing
 */
void Timer2On(uint8_t period);  

/*
 * Function: void Timer2Off( void )
 * --------------------
 * Timer2 will cease to increment.  
 *
 * returns: Nothing
 */
void Timer2Off( void );

/*
 * Function: void Timer3Init( bool interrupts, uint8_t prescaler, bool clksource )
 * --------------------
 * Timer 3 is initialized.
 *
 * returns: Nothing
 */
void Timer3Init( uint8_t interrupts, uint8_t prescaler, uint8_t clksource );

/*
 * Function: void Timer3On(uint8_t RegHigh, uint8_t RegLow)
 * --------------------
 * Timer 3 will begin to increment.  16 bit register shall
 * be passed in.
 *
 * returns: Nothing
 */
void Timer3On(uint8_t RegHigh, uint8_t RegLow);

/*
 * Function: void Timer3Off( void )
 * --------------------
 * Timer 3 will cease to increment.  
 *
 * returns: Nothing
 */
void Timer3Off( void );

/*
 * Function: void Timer4Init( bool interrupts, uint8_t prescaler, uint8_t clksource )
 * --------------------
 * Timer 4 is initialized.  It is during initialization that it
 * shall be determined whether or not interrupt shall be generatred
 * on overflow.  
 *
 * returns: Nothing
 */
void Timer4Init( uint8_t interrupts, uint8_t prescaler, uint8_t clksource );

/*
 * Function: void Timer4On( uint8_t period )
 * --------------------
 * To turn ON timer 4.  The perior register will 
 * count up from 0x00 until the count value equals
 * the user-defined period value.  At this point,
 * the interrupt is triggered and the timer value 
 * automatically reset.  
 *
 * returns: Nothing
 */
void Timer4On( uint8_t period );

/*
 * Function: void Timer4Off( void )
 * --------------------
 * Timer 4 will cease to increment.  
 *
 * returns: Nothing
 */
void Timer4Off( void );

#endif
/* END OF FILE */