/******************************************************************************
*   FILE: struct.h
*
*   PURPOSE: Structures used throughout the code base are defined here.  
*
*   DEVICE: PPIC18F66K22
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
#ifndef __STRUCT_H_
#define __STRUCT_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file

extern struct GlobalInformation         //Structure to pass information that is shared among modules
{
    bool        bat_low;                //Flag will be set if battery voltage detected too low
    bool        wakeedge;               //Set this flag if we get an interrupt on INTx pin (feature no longer used)
    float       battery_voltage;        //Container for measured battery voltage
    
    uint16_t    tick10ms;               //Increases every 50mili tick1000monds 
    uint16_t    tick100ms;              //Used to keep track of passing deci-tick1000monds
    uint16_t    tick1000ms;             //Use this to tick tick1000monds -- might not be used for audio interface board

    uint8_t     int_temp_val;           // Integer value of temperature to be displayed on the screen
    uint8_t     int_hum_val;            // Integer value of humidity to be displayed on the screen
    
} GlobalInformation;


#endif