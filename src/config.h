/******************************************************************************
*   FILE: config.h
*
*   PURPOSE: Configuration file specific to the processor being used and the 
*           underlying hardware. 
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "main.h"
#include "struct.h"

/* PREPROCESSOR CALCULATION TO DETERMINE TIMER4'S INCREMENT RATE */
#define MCU_OSC_FRQ         8000000.0                  // Oscillator used for MCU
#define OSC_DIV4            (MCU_OSC_FRQ/4.0)          // Oscillator used for MCU

/* REGISTER VALUES FOR 10MS TIME BASE */         
#define TMR0_INTUP_SETTING  1                                                           // 1 = Caused interrupts, 0 = do not cause interrupts
#define TMR0_PRESCALER      8                                                           // Options are 1, 2, 4, 8, 16, 32, 128, or 256
#define TMR0_INC_FREQ       (OSC_DIV4/TMR0_PRESCALER)                                   // Effective rate at which the timer increments
#define HEART_BEAT_MS       20.0                                                        // Interrupt at this periodicity (mili-seconds)
#define TMR0_TICKS          ((HEART_BEAT_MS/1000.0)*TMR0_INC_FREQ)                      // How many timer ticks between interrupts
#define TMR0HIGH            (uint8_t)((65535-TMR0_TICKS)/256)                           // Value to be loaded into the 8-bit register
#define TMR0LOW             (uint8_t)(TMR0_TICKS-(256*(uint8_t)(TMR0_TICKS/256)))        // Module implementation to obtain register low value

/* DEFINE CODE VERSION NUMBER */
#define MAJVER                  0x00
#define MINVER                  0x01
#define BUGVER                  0x00

/* DEFINITIONS RELATED TO DISPLAY */
// #warning "Display update time shall be >> than display backlight time!"

#define SECONDS_BETWEEN_DISP_UPDATE         5               // Amount of time (in seconds) that will pass between display updates            
#define SECONDS_DISP_BACKLIGHT_ON           1               // Amount of time (in seconds) that the display backlight will be on once enabled

/* Processor defines for I2C sensor */
#define SEL_SENSOR2_BIT                     LATDbits.LATD7
#define SELECT_SENSOR_1                     0
#define SELECT_SENSOR_2                     1

/* PIN DEFINES FOR LCD SCREEN */    //TODO has been updated
#define DISP_ENABLE                         LATBbits.LATB4          // Active low signal for turning on display
#define DISP_RESET                          LATCbits.LATC1          // Active low signal to reset the display
#define DISP_REG_SEL                        LATCbits.LATC0          // Register select signal. 0 = instruction, 1 = data
#define DISP_SPI_CS                         LATCbits.LATC2          // Display Chip Select Signal 

/* DEFINES FOR LED PINS */  //TODO has been updated
#define health_led                          LATBbits.LATB5          // For driving health LED  

/* DEFINES FOR PUSH BUTTONS */      //TODO has been updated
#define PB1_GPIO                            PORTBbits.RB0           // Inputs from buttons
#define PB2_GPIO                            PORTBbits.RB1
#define BUTTON_PUSHED                       1                       // State of input when button pushed
#define BUTTON_RELEASED                     !(BUTTON_PUSHED)
#define BUTTON_DEBOUNCE_TIME                0.140                   // Should be in increments of timer resolution (i.e. 20ms)
#define BUTTON_DEBOUNCE_TICKS               (uint8_t)(BUTTON_DEBOUNCE_TIME/0.02)

/* Defines for state machine */
#define SENSOR_REFRESH_RATE_SECONDS         1
#define COUNTS_20MS_BETWEEN_SENSOR_UPDATE   (uint8_t)(SENSOR_REFRESH_RATE_SECONDS/0.02)

/* DEFINE VARIOUS PIN FUNCTIONS */
#define output              0           //Define the output pin direction setting
#define input               1

/* INTUITIVE TIMER NAMES */
#define TIMER1              1
#define TIMER2              2
#define TIMER3              3
#define TIMER4              4
#define TIMER5              5
#define TIMER6              6
#define TIMER7              7
#define TIMER8              8
#define TIMER9              9   
#define TIMER10             10

/* IO MNEMONICS */
#define LED_ON              0
#define LED_OFF             1
#define DISPLAY_OFF         1
#define DISPLAY_ON          0

#endif