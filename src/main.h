/******************************************************************************
*   FILE: main.h
*
*   PURPOSE: Header file for main.c
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
#ifndef __MAIN_H_
#define __MAIN_H_

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "i2c.h"
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "adc.h"
#include "dispdriver.h"

#define TEMP 65577

/********************************************************
*FUNCTION: void tick100mDelay( uint16_t ticks )
*PURPOSE: Much more accurate timer that works off interrupts
            User must define how many 1/10s of a tick1000mond he/she
            wishes to pass
*PRECONDITION: Timers must be set up and running in order for this to work
*POSTCONDITION: tick100ms * 1/10s of a tick1000mond have passed.
*RETURN: Nothing
********************************************************/
void tick100mDelay( uint16_t ticks );

/********************************************************
*FUNCTION: void tick10msDelay( uint16_t ticks )
*PURPOSE: Much more accurate timer that works off interrupts
            User passes in how many 1/50s he/she wishes to pass 
*PRECONDITION: Timer0 set up and running and set to interrupt
*POSTCONDITION: Blocking delay inserted
*RETURN: Nothing
********************************************************/
void tick10msDelay( uint16_t ticks );

/********************************************************
*FUNCTION: void SetUp( void );
*PURPOSE: Set up the PIC I/O and etc...
*PRECONDITION: PIC not configured
*POSTCONDITION: PIC I/O Configured
*RETURN: Nothing
********************************************************/
void SetUp( void );

/********************************************************
*FUNCTION: void TempHumidityInitialize(void)
*PURPOSE: Initialize the Temp/Humidity sensor.  
*       Temp and Humidity is linearly extrapolated from line 
*       equations that can be built using calibration data 
*       stored in internal registers.  This function pulls in
*       these calibration constants, and 'builds' the line 
*       equations.  
*PRECONDITION: PIC must be completely setup and I2C bus configured
*POSTCONDITION: Temp/Humidity Line Equations Developed
*RETURN: Nothing
********************************************************/
void TempHumidityInitialize(void);      

/********************************************************
*FUNCTION: void ReportTH( void )
*PURPOSE: Report temperature and humidity over CAN bus
*PRECONDITION: I2C and CAN must be configured
*POSTCONDITION: Temperature and Humidity data is broadcast
*           out over the CAN bus
*RETURN: Nothing
********************************************************/
void UpdateTH( void );  

/********************************************************
*FUNCTION: void BatteryStatus( void )
*PURPOSE: Measure battery voltage and determine if value
* is too low
*PRECONDITION: Appropriate analog channel shall be enabled
* and A2D shall be setup and configured.  
*POSTCONDITION: Battery voltage measured and battery state
* (either fine or too low) is known
*RETURN: Nothing
********************************************************/
void BatteryStatus( void );

#endif