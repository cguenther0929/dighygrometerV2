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
#ifndef __STRUCT_H
#define __STRUCT_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file

extern struct GlobalInformation         //Structure to pass information that is shared among modules
{
    /* Display Related  */
    bool        disp_tmr_active;
    uint8_t     disp_seconds_ctr;
    uint8_t     next_disp_update;       // When tick1000ms matches this value, update the display again
    uint8_t     next_disp_shtdn;        // When tick1000ms matches this value, turn display backlight off
    
    /* COUNTER RELATED */
    uint16_t    tick20ms;               // Counts 0-4, resets every 100ms. 
    uint16_t    tick100ms;              // Counts 0-4, resets every 500ms.  
    uint16_t    tick500ms;              // Counts 0-1, resets every 1000ms.  
    uint16_t    tick1000ms;             // Counts 0-56, resets every min.

    /* BUTTON RELATED */
    uint8_t     up_btn_press_ctr;
    uint8_t     lo_btn_press_ctr;
    uint8_t     btn_both_press_ctr;
    bool        up_btn_pressed;
    bool        lo_btn_pressed;
    bool        both_btns_pressed;

    /* Sensor Related */
    uint16_t  tick20msloopctr
    float     rh_value_1;
    float     rh_offset_1;              // Stores the RH offset of sensor 1 upon calibration request
    float     temp_value_1;
    
    float     rh_value_2;
    float     rh_offset_2;              // Stores the RH offset of sensor 2 upon calibration request 
    float     temp_value_2;
    
} GlobalInformation;


#endif