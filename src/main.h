/******************************************************************************
*   FILE: main.h
*
*   PURPOSE: Header file for main.c
*
*   TODO: Determine why ARRAY_LENGTH cannot be used when defining 
*           EvaluateState.  The number 17 shall be typed, or else 
*           compile errors are present.
*
*   NOTE:
*
******************************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"     //Project specific header file
#include "struct.h"
#include "sensor.h"
#include "isr.h"
#include "timer.h"
#include "spi.h"
#include "disp.h"
#include "adc.h"

/* Main Application States */
typedef enum
{
    STATE_IDLE,
    STATE_GRAB_SENSOR_DATA,
    UPDATE_DISPLAY,
    STATE_CALIBRATE,
    STATE_CLEAR_CALIBRATE
} App_State;

typedef enum{
    DISP_TMR_DISABLE,
    DISP_TMR_ENABLE,
    DISP_TMR_CNT,
    DISP_TMR_RST
} Disp_Actions;


/* STATES FOR FSM */   //TODO need to update
// #define STATE_IDLE         0           // Idle app_state with dispaly on -- RX Continuous
// #define STATE_IDLE_NO_DISP      1           // Idle app_state with display off -- RX Continuous
// #define STATE_SELECT_RECIPENTS  2           // Scrolling through a list of possible recipents 
// #define STATE_SELECT_MSG        3           // Scrolling through list of possible messages
// #define STATE_CONFIRM_MSG       4           // Confirm message to send
// #define STATE_TRANSMIT_MSG      5           // Transmitting message  

/* DEFINES FOR TIMER ACTION*/
// #define DISP_TMR_RST        0
// #define DISP_TMR_CNT        1
// #define DISP_TMR_ENABLE     2
// #define DISP_TMR_DISABLE    3

/*
 * Function:  void tick100msDelay( uint16_t ticks );
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 100ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
void tick100msDelay( uint16_t ticks );

/*
 * Function:  void tick20msDelay( uint16_t ticks );
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 20ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
void tick20msDelay( uint16_t ticks );

/*
 * Function:  void SetUp( void );
 * --------------------
 * SetUp routine covering all aspects of the application. 
 * Processor GPIOs are configured.  Peripherials are 
 * configured.  ADC is configured.  System timebase 
 * and general timing parameters are configured.  
 *
 * returns: Nothing 
 */
void SetUp( void );

/*
 * Function:  float GetBatteryVoltage ( void );  //TODO remove
 * --------------------
 * Retrieve batter voltage.  ADC must first be fully
 * configred/initialized.   
 *
 * returns: Battery Voltage as float
 */
// float GetBatteryVoltage ( void );   

/*
 * Function:  void EvaluateState( void ); 
 * --------------------
 * Evaluate current app_state and whethere or not the FSM 
 * shall advanced.   
 *
 * returns: Nothing
 */
void EvaluateState( void );

/*
 * Function:  void EvaluateButtonInputs ( void )
 * --------------------
 * Evaluate whethere or not a button has been pushed
 *
 * returns: Nothing
 */
void EvaluateButtonInputs ( void ); 

/*
 * Function:  void DisplayDwellTmr( uint8_t action)     
 * --------------------
 * Handle display dwell timer.  This timer is responsible
 * for powering off the display backlight after a period of 
 * no activity (no buttons or received messages)
 *
 * returns: Nothing
 */
// void DisplayDwellTmr( uint8_t action);  

/*
 * Function:  void PrintSplashScreen( void )
 * --------------------
 * Print information like model name/FW version/
 * battery voltage before the device enters into 
 * the FSM. computes an approximation of pi using:
 *
 * returns: Nothing
 */
void PrintSplashScreen( void );     

#endif