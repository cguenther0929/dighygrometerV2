/******************************************************************************
*   FILE: i2c.h
*
*   PURPOSE: Header file for i2c.c
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
#ifndef __I2C_H_
#define __I2C_H_

/* PULL IN OTHER H FILES AS NECESSARY */
#include <xc.h>
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
#define Oscillator      500000                            //Device oscillator value
#define I2CCLOCK        100000                            //I2C Clock Value -- Need to make sure this value isn't too high for given FOSC value 
// #define BaudValue       ((Oscillator/(4.0*I2CCLOCK)) - 1.0)     //Value for SSPADD register (master-mode only)          
#define BaudValue       10     //Value for SSPADD register (master-mode only)          
#define i2cdelay        10

// #if ((BaudValue >> 8) & 0X01)
//     #warning "Baud Value has value in bit 8"
// #endif

/* DEFINE SPECIAL SFR NAMES FOR I2CxSTAT REGISTER */
//SSP1CON2 bits  -- these have been reviewed and confirmed 8/27/17
#define I2CACKBit       SSP1CON2bits.ACKDT       //Intelligent mnemonic for defining the ACK/NACK bit.  Master Receive mode only.  Set then gen ack via I2CGenACK
#define I2CGenACK       SSP1CON2bits.ACKEN       //Intelligent mnemonic for generating an ACK/NACK
#define I2CGenStart     SSP1CON2bits.SEN         //Intelligent mnemonic for generating a start condition.  Automatically cleared by HW
#define I2CGenStop      SSP1CON2bits.PEN         //Intelligent mnemonic for generating a stop condition
#define I2CRepStart     SSP1CON2bits.RSEN        //Intelligent mnemonic for generating a repeated start condition
#define I2CRecEnable    SSP1CON2bits.RCEN        //Intelligent mnemonic for defining we wish to receive data
#define I2CACKStat      SSP1CON2bits.ACKSTAT     //Intelligent mnemonic for determined if the slave sent an ACK/NACK

// SSP1StatBits -- these have been reviewed and confirmed 8/27/17
#define I2CTXBusy       SSP1STATbits.R_nW       //Intelligent mnemonic indicating a transmission is busy  -- Start at line 14933 is pick18f66k22.h
#define I2CBufFull 	    SSP1STATbits.BF		    //Define special SFR name for Transmit Buffer Full Status Bit
#define I2CStopDetect   SSP1STATbits.P          //Intelligent mnemonic indicating a stop condition was detected
#define I2CStartDetect  SSP1STATbits.S          //Intelligent mnemonic indicating a start condition was detected

#define I2C_Active      (RW1 + SEN1 + RSEN1 + PEN1 + RCEN1 + ACKEN1) //<< 4) | (RSEN1 << 3) | (PEN1 << 2) | (RCEN1 << 1) | ACKEN1)) //Indicates when I2C module is active p292 of datasheet 

/* I2C BITS AND POLARITY */
#define I2CREAD			1					    //Define the read bit to be high
#define I2CWRITE		0					    //Define the write bit to be low
#define ACK             0                       //An I2C ACK is active low
#define NACK            1                       //An I2C NACK is active high


/* DECLARE ALL FUNCTIONS */
/********************************************************
*FUNCTION: void I2Cinit( void )
*PURPOSE: To initalize the i2c bus module on the PIC
*PRECONDITION: NONE 
*POSTCONDITION: i2c module is ready to use as Master Mode
*				7 bit address mode
*RETURN: NOTHING
********************************************************/
void I2Cinit( void );

/********************************************************  TODO update the description
*FUNCTION: uint32_t ReadI2C(uint8_t addr)
*PURPOSE: To read a byte of data over the I2C bus.  
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass functions proper
*       addresses.  
*POSTCONDITION: BYTE of data will be returned to application
*RETURN: The BYTE that was read over the bus
********************************************************/
uint32_t I2CRead(uint8_t baseaddress);

/********************************************************  
*FUNCTION: void I2CWrite_16(uint8_t baseaddress, uint8_t subaddress, uint16_t senddata)
*PURPOSE: Write 16 bits of data over I2C bus.  
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass in proper
*       addresses and data to be sent
*POSTCONDITION: BYTE of data written over bus
*RETURN: Nothing
********************************************************/
void I2CWrite_16(uint8_t baseaddress, uint8_t subaddress, uint16_t senddata);  

/********************************************************  
*FUNCTION: void I2CWrite_8(uint8_t baseaddress, uint8_t subaddress, uint8_t senddata)
*PURPOSE: Write BYTE of data over the I2C bus.  
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass in proper
*       addresses and data to be sent
*POSTCONDITION: BYTE of data written over bus
*RETURN: Nothing
********************************************************/
void I2CWrite_8(uint8_t baseaddress, uint8_t subaddress, uint8_t senddata);  

/********************************************************  
*FUNCTION: void I2CWrite_SetPointer(uint8_t baseaddress, uint8_t ptr_address)
*PURPOSE: Set the pointer address for the HDC1080DMBT Hygrometer IC 
*PRECONDITION: I2C module must be configured.
*       Bus needs to be idle. Must pass in proper
*       addresses and pointer address 
*POSTCONDITION: Register pointer configured in HDC1080
*RETURN: Nothing
********************************************************/
void I2CWrite_SetPointer(uint8_t baseaddress, uint8_t ptr_address);

#endif