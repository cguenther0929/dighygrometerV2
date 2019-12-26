/******************************************************************************
*   FILE: config.h
*
*   PURPOSE: Configuration file specific to the processor being used and the 
*           underlying hardware. 
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
#ifndef __CONFIG_H_
#define __CONFIG_H_

//#include "type.h"       //Needed for defined functions
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

/* DEFINE CODE VERSION NUMBER */
#define MAJVER              0x00
#define MINVER              0x00
#define BUGVER              0x01

/* DEFINES FOR STATUS GPIO */
#define PULSEOUT            LATBbits.LATB5

/* DEFINES FOR POWER CONTROL*/  
#define DISP_PWR_EN_n       LATEbits.LATE7      //Assert low to enable power to the display
#define PIEZ_KILL           LATEbits.LATE6      //Assert high to kill stored power keeping power to the MCU.
#define MCU_PWR_LATCH       LATEbits.LATE5      //Assert high to keep power to the MCU    

/* DEFINES FOR LCD PINS*/
#define DISP_BYTE           LATD                //Port that connects to dispaly
#define DISP_RS             LATEbits.LATE2      //Register Select signal. RS=0=Command, RS=1=Data
#define DISP_RW             LATEbits.LATE1      //Read/Write. R/W=1=Read; R/W=0=Write 
#define DISP_E              LATEbits.LATE0      //Operation enable (falling edge to trigger)
#define disp_write          0
#define disp_read           1
#define disp_command        0
#define disp_data           1

/* DEFINE UNUSED PINS */
#define NC1                 LATAbits.LATA1


/* REGISTER VALUES FOR TIME BASE */         
#define TMR0HIGH            251                 //Defined assuming 500kHz internal oscillator and prescaler of 1 interrupt every 10ms 
#define TMR0LOW             29                  //Defined assuming 500kHz internal oscillator and prescaler of 1 interrupt every 10ms

/* I2C ADDDRESSES FOR HUMIDITY SENSOR */
#define THBaseAddr          0x40            // 7bit address of chip is 0b1000000 -- Full Address is 0b1000000 + R/#W
#define THConfigReg         0x02            // Address for configuration register
#define THValuePointer      0x00            // Pointer address when reading temperature / humidity

// The following is the configuration register of the part, whose reset value is 0x1000
                                            // 0b0001 | 0000 | 0000 | 0000 = 0x1000
#define THConfigVal         0x1000          // [RST(15)][RESERVED(14)][HEAT(13)][MODE(12)] | [BAT STATE(11)][TRES(10)][HRES(9:8)] | RESERVED (7:0)


/* DEFINE VARIOUS PIN FUNCITONS */
#define output              0           //Define the output pin direction setting
#define input               1

/* BATTERY STATUS DEFINES */
#define BAT_VOLT_MIN        2.5         //Battery voltage at or below this value will trip the alert message          

#endif