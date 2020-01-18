/******************************************************************************
*	FILE: spi.h
*
*	PURPOSE: Header file for SPI communication
*			Target device: PIC16F15xx 8 bit MCU.   
*
*	TODO:
*	
*   NOTE:
*
******************************************************************************/

#ifndef __SPI_H
#define __SPI_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "main.h"

#define spidelay        50             //Define a small delay


/*
 * Function:  void SPI1Init( void )
 * --------------------
 * Initialize the SPI1 module.  
 * 
 * returns: Nothing
 */
void SPI1Init( void );

/*
 * Function:  void SPI2Init( void )
 * --------------------
 * Initialize the SPI2 module.  
 * 
 * returns: Nothing
 */
void SPI2Init( void );  

/*
 * Function: void RFMSPI2Write(uint8_t addr, uint8_t data)
 * --------------------
 * Write the byte of data at the address defined.  
 * SPI2 module shall first be configured.    
 * 
 * returns: Nothing
 */
// void RFMSPI2Write(uint8_t addr, uint8_t data);

/*
 * Function: void RFMSPI2WriteBurst(uint8_t addr, const char * data, uint8_t len)
 * --------------------
 * Write multiple bytes of data starting at the deinfed address.  
 * The number of byes to be written is defined by the len parameter.  
 * 
 * returns: Nothing
 */
// void RFMSPI2WriteBurst(uint8_t addr, const char * data, uint8_t len);

/*
 * Function: void RFMSPI2ReadBurst(uint8_t addr, char * data, uint8_t len)
 * --------------------
 * Read multiple bytes of data starting at the deinfed address.  
 * The number of byes to be read is defined by the len parameter.  
 * 
 * returns: Nothing
 */
// void RFMSPI2ReadBurst(uint8_t addr, char * data, uint8_t len);

/*
 * Function: uint8_t RFMSPI2Read(uint8_t addr)
 * --------------------
 * Read a byte of data at the address defined.  
 * SPI2 module shall first be configured.    
 * 
 * returns: Byte of data extracted from SPI
 * transaction
 */
// uint8_t RFMSPI2Read(uint8_t addr);     

/*
 * Function: void DispSPI1Write(uint8_t data)
 * --------------------
 * Write the byte of data to the display module
 * SPI1 module shall first be configured.  
 * 
 * returns: Nothing 
 */
void DispSPI1Write(uint8_t data);

#endif
/* END OF FILE */