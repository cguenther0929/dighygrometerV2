/******************************************************************************
 *   FILE: sensor.h
 *
 *   PURPOSE: Header file for sensor.c.  Function declared herein 
 *          are specifi to the SI7020 hygrometer sensor 
 *
 *   TODO: 
 *
 *   NOTE:
 *
******************************************************************************/
#ifndef __SENSOR_H
#define __SENSOR_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/*
 * The following equation is listed on page 22/35 in the SI7020 datasheet.
 * This formula shall be used when needing to extract the temperature data
 * from the sensor.
 *  
 *              +--+              +---+
 *    TEMP_C =  | 175.72 * Temp_Code  |
 *              | +----------------+  | - 46.8
 *              |       65536         |
 *              +---+             +---+
 *  
 * Converting the above equation so that temperature in deg F
 * is calculated results in the following equation
 * 
 *              +--+              +---+
 *    TEMP_F =  |      Temp_Code      |
 *              | +----------------+  | - 52.24
 *              |      207.1952       |
 *              +---+             +---+
 *
 * For determing RH, the following equation shall be 
 * used.
 * 
 *              +--+              +---+
 *       %RH =  |    125 * RH_code    |
 *              | +----------------+  | - 6
 *              |       65536         |
 *              +---+             +---+
 * 
 */

#define SI7020_BASE_ADDRESS             0x40

#define SI7020_MEAS_HUM_HOLD_MASTER     0xE5
#define SI7020_MEAS_NO_MASTER           0xF5
#define SI7020_MEAS_TMP_HOLD_MASTER     0xE3
#define SI7020_MEAS_TMP_NO_MASTER       0xF3
#define SI7020_MEAS_TMP_PREV_RH_MEAS    0xE0

#define SI7020_RESET_COMMAND            0xFE
#define SI7020_WRT_USR_REG1             0xE6
#define SI7020_READ_USR_REG1            0xE7
#define SI7020_WRT_HEATER_CTRL_REG      0x51
#define SI7020_READ_HEATER_CTRL_REG     0x11

#define SI7020_ID_BYTE1_1               0xFA            // Reading the ID byte require the CRC byte be read!  p19/35 in datasheet
#define SI7020_ID_BYTE1_2               0x0F

#define SI7020_ID_BYTE2_1               0xFC
#define SI7020_ID_BYTE2_2               0xC9

#define SI7020_READ_FW_VER_1            0x84
#define SI7020_READ_FW_VER_1            0xB8

#define I2C_WRITE_BIT                   0x01            // Sample write == 1000000 + 1
#define I2C_READ_BIT                    0x00            // Sample read == 1000000 + 0

#define SI7020_HTRE_3D09MA              0x00            // Heater control register on p26/38 -- Heater bits are D3:D0 (lowest nibble)
#define SI7020_HTRE_9D18MA              0x01
#define SI7020_HTRE_15D24MA             0x02
#define SI7020_HTRE_27D37MA             0x04
#define SI7020_HTRE_51D69MA             0x08
#define SI7020_HTRE_94D20MA             0x0F



/*  TODO comment
 * These function must be defined
 *
 */  
void EnableHeater( uint8_t level );


/*  TODO comment
 * This function must be defined
 *
 */  
void DisableHeater( void );


/*  TODO comment
 * 
 * 
 */ 
void SelectSensor (uint8_t sensor_select);

#endif