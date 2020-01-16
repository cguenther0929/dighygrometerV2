/******************************************************************************
 *   FILE: i2c.h
 *
 *   PURPOSE: Header file for i2c.c.  This is simply the low level driver,
 *          and routines related to the sensor are located within 
 *
 *   TODO: Need to clean up function comments
 *
 *   NOTE:
 *
******************************************************************************/
#ifndef __I2C_H
#define __I2C_H

/* PULL IN OTHER H FILES AS NECESSARY */
#include <xc.h>
#include "sensor.h"
#include "disp.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/******************************************************************************
    This note contemplates master mode only. See p.311 of the datasheet
    The following actions shall cause the SSPxIF bit to be set (i.e. interrupt is generated)
    - Start Condition
    - Stop Condition
    - Data transfer byte (transmitted/received) compelted 
    - Acknowledge transmitted
    - Repeated start 
******************************************************************************/


/* MASTER MODE CLOCK RATE */
#define Oscillator      8000000                           //Device oscillator value
#define I2CCLOCK        100000                            //I2C Clock Value -- Need to make sure this value isn't too high for given FOSC value 
// #define BaudValue       ((Oscillator/(4.0*I2CCLOCK)) - 1.0)     //Value for SSPADD register (master-mode only)          
#define BaudValue       10     //Value for SSPADD register (master-mode only)          
#define i2cdelay        10

// #if ((BaudValue >> 8) & 0X01)
//     #warning "Baud Value has value in bit 8"
// #endif

/* DEFINE SPECIAL SFR NAMES FOR I2CxSTAT REGISTER */
//SSP1CON2 bits   TODO can likely finish much of the following.  
// #define ACKDT2       SSP1CON2bits.ACKDT       //Intelligent mnemonic for defining the ACK/NACK bit.  Master Receive mode only.  Set then gen ack via ACKEN2
// #define ACKEN2       SSP1CON2bits.ACKEN       //Intelligent mnemonic for generating an ACK/NACK
// #define      SSP1CON2bits.SEN         //Intelligent mnemonic for generating a start condition.  Automatically cleared by HW
// #define RCEN2      SSP1CON2bits.PEN         //Intelligent mnemonic for generating a stop condition
// #define RCEN2      RCEN2  
// #define RSEN2     SSP1CON2bits.RSEN        //Intelligent mnemonic for generating a repeated start condition
// #define RCEN2    SSP1CON2bits.RCEN        //Intelligent mnemonic for defining we wish to receive data
// #define ACKSTAT2      SSP1CON2bits.ACKSTAT     //Intelligent mnemonic for determined if the slave sent an ACK/NACK

// SSP1StatBits 
#define I2CTXBusy       SSP1STATbits.R_nW       //Intelligent mnemonic indicating a transmission is busy  -- Start at line 14933 is pick18f66k22.h
#define I2CBufFull 	    SSP1STATbits.BF		    //Define special SFR name for Transmit Buffer Full Status Bit
#define I2CStopDetect   SSP1STATbits.P          //Intelligent mnemonic indicating a stop condition was detected
#define I2CStartDetect  SSP1STATbits.S          //Intelligent mnemonic indicating a start condition was detected

#define I2C_Active      (RW2 + SEN2 + RSEN2 + RCEN2 + RCEN2 + ACKEN2) //<< 4) | (RSEN2 << 3) | (RCEN2 << 2) | (RCEN2 << 1) | ACKEN2)) //Indicates when I2C module is active p292 of datasheet 

/* I2C BITS AND POLARITY */
#define ACK             0                       // An I2C ACK is active low
#define NACK            1                       // An I2C NACK is active high


/********************************************************
*FUNCTION: void I2Cinit( void )
*PURPOSE: To initalize the i2c bus module on the PIC
*PRECONDITION: NONE 
*POSTCONDITION: i2c module is ready to use as Master Mode
*				7 bit address mode
*RETURN: NOTHING
********************************************************/
void I2Cinit( void );


uint8_t I2CRead_8b(uint8_t baseaddress, uint8_t command);  //TODO need to comment

/********************************************************  TODO update the description
*FUNCTION: uint32_t ReadI2C (uint8_t addr)
*PURPOSE: To read a byte of data over the I2C bus.  
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass functions proper
*       addresses.  
*POSTCONDITION: BYTE of data will be returned to application
*RETURN: The BYTE that was read over the bus
********************************************************/
uint16_t I2CRead_16b(uint8_t baseaddress, uint8_t command);



/********************************************************  TODO update description
*FUNCTION: void I2CWrite_8 (uint8_t baseaddress, uint8_t subaddress, uint8_t senddata)
*PURPOSE: Write BYTE of data over the I2C bus.  
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass in proper
*       addresses and data to be sent
*POSTCONDITION: BYTE of data written over bus
*RETURN: Nothing
********************************************************/
void I2CWrite_8b(uint8_t baseaddress, uint8_t command, uint8_t data);

#endif