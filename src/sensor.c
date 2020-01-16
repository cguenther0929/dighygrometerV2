/******************************************************************************
 *   FILE: sensor.c
 *
 *   PURPOSE: Source file for all hygrometer related routines.  
 *          Sensor used for this application: Silicon Labs: SI7020-A20-GM1R
 *
 *   TODO:  
 *
 *   NOTE: 
 *
******************************************************************************/
#include "sensor.h"

void SelectSensor (uint8_t sensor_select) {
    SelectSensor = SELECT_SENSOR_1 ? (SEL_SENSOR2_BIT = SELECT_SENSOR_1):(SEL_SENSOR2_BIT = SELECT_SENSOR_2);
}

void EnableHeater( uint8_t level ) {
    uint8_t temp_byte = 0x00;
    
    /* Perfrom read/modify/write to enable heater bit */
    temp_byte = I2CRead_8b(SI7020_BASE_ADDRESS, SI7020_READ_USR_REG1);
    I2CWrite_8b(SI7020_BASE_ADDRESS, SI7020_WRT_USR_REG1 , temp_byte | 0x04);    // Heater OR mask 0000 0_1_00
    
    I2CWrite_8b(SI7020_BASE_ADDRESS, SI7020_WRT_HEATER_CTRL_REG, level);    


}

void DisableHeater( void ) {
    uint8_t temp_byte = 0x00;

    /* Perfrom read/modify/write to enable heater bit */
    temp_byte = I2CRead_8b(SI7020_BASE_ADDRESS, SI7020_READ_USR_REG1);
    I2CWrite_8b(SI7020_BASE_ADDRESS, SI7020_WRT_USR_REG1 , temp_byte & 0xFB);    // Heater mask 1111 1_0_11
    
    I2CWrite_8b(SI7020_BASE_ADDRESS, SI7020_WRT_HEATER_CTRL_REG, SI7020_HTRE_3D09MA);   //To be extra safe, set to lowest current level    
}