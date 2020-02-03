/******************************************************************************
 *	FILE: disp.c
 *
 *	PURPOSE: Contains helper functions for communication with the 
 *          display.  Taret device: PIC18F66K22, advanced 8 bit MCU.
 *
 *	AUTHOR: Clinton Guenther
 *
 *	TODO:	      
 *
******************************************************************************/

#include "disp.h"

struct GlobalInformation gblinfo;

void DispInit ( void ) {
    uint8_t i;
    
    DISP_REG_SEL = 0;
    DISP_ENABLE = DISPLAY_ON;
    tick100msDelay(1);
    
    DISP_RESET = 0;
    tick100msDelay(1);
    DISP_RESET = 1;
    tick100msDelay(1);


    DispSPI1Write(0x30);            // Send a few wakeup commands (taken from example in datasheet)
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x30);
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x30);
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x39);        // Function set
    for (i=0;i<20;i++);

    DispSPI1Write(0x14);        // Internal oscillator frequency
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x56);        // power control -- taken directly from datasheet -- seems to work as-is
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x6D);        // follower control -- -- taken directly from datasheet -- seems to work as-is
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x70);        // Contrast set.  The lower the second nibble, the harder to see. (i.e. 0x7A will have brighter contrast)
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x0C);        // Entire display ON
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x06);        // Disp entry mode 
    for (i=0;i<20;i++);

    DispClear();
    
    DispCursorHome();

}

void DispSetContrast(uint8_t percentage) {
    uint8_t i;
    uint8_t contrast_value; 
    float flt_contrast_value; 

    DISP_ENABLE = DISPLAY_ON;

    DISP_REG_SEL = 0;
    for (i=0;i<20;i++);

    flt_contrast_value = (float)(percentage*0.16);

    contrast_value = (uint8_t)(flt_contrast_value);

    if(contrast_value > 15) 
        (contrast_value = 15);

    contrast_value = (uint8_t)(0x70 | contrast_value);
    
    DispSPI1Write(contrast_value);

}

void DispRefresh( void ) {
    DispClear();
    DispCursorHome();
}

void DispClear( void ) {
    DISP_REG_SEL = 0;
    DispSPI1Write(0x01);
}

void DispCursorHome( void ) {  
    DISP_REG_SEL = 0;
    DispSPI1Write(0x02);
}

void DispLineOne (void ) {
    DISP_REG_SEL = 0;
    DispSPI1Write(0x80);        // Data format D[7:0] = 0b 1 A6 A5 A4 A3 A2 A1 A0.  For 0x00 = 0b 1000 0000 0x80
}

void DispLineTwo (void ) {
    DISP_REG_SEL = 0;
    DispSPI1Write(0xA8);        // Data format D[7:0] = 0b 1 A6 A5 A4 A3 A2 A1 A0.  For 0x40 = 0b 1010 1000 0xA8
}


void DispWriteChar (uint8_t c) {
    uint8_t i;
    
    DISP_REG_SEL = 1;
    for (i=0;i<50;i++);
    DispSPI1Write(c);

    DISP_REG_SEL = 0;
    
}

void DispWtLnOne ( char * y ) {
    uint8_t char_ctr = 0;
    DispLineOne();

    while(*y != '\0' && char_ctr < 16){
        DispWriteChar(*y);
        y++;                           //Increment the pointer memory address
        char_ctr++;
    }

    while(char_ctr < 16){
        DispWriteChar(' ');
        char_ctr++;
    }

}

void DispWtLnTwo ( char * y ) {
    uint8_t char_ctr = 0;
    DispLineTwo();

    while(*y != '\0' && char_ctr < 16){       
        DispWriteChar(*y);
        y++;                           //Increment the pointer memory address
        char_ctr++;
    }

    while(char_ctr < 16){
        DispWriteChar(' ');
        char_ctr++;
    }

}

void DispWriteString(char * y) {

    while(*y != '\0'){
        DispWriteChar(*y);
        y++;                           //Increment the pointer memory address
    }

}

void PrintDecimalNumber (uint16_t number) {
    char temphex[5];        //Define the array that will hold the ASCII values
    uint8_t i;                //Use this as a counter
    uint8_t decimal_count;    //This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = sprintf(temphex, "%u", number); //u tells the function we want an unsigned decimal number

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        DispWriteChar(temphex[i]);
    }

}

void DispWriteFloat (float number) {
    char temphex[8];        //Define the array that will hold the ASCII values
    uint8_t i;                //Use this as a counter
    uint16_t j;
    uint8_t decimal_count; //This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = sprintf(temphex, "%.4f", number);   //f tells the function we want to print a float value

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        DispWriteChar(temphex[i]);
    }
}

void DispWrite8b (uint8_t number) {
    uint16_t temp;      //Use for temporary value
    uint8_t i;         //Counter value
    for(i = 8; i > 0 ; i--) {
        temp = number >> (i - 1);
        temp = temp & 0x01;
        temp = temp + 0x30;         //Turn this into an ASCII value
        DispWriteChar(temp);
    }
}

void GetNewDisplayRefreshTime ( void ) {
    if((gblinfo.tick1000ms + SECONDS_BETWEEN_DISP_UPDATE) > 59) {
        gblinfo.next_disp_update = (gblinfo.tick1000ms + SECONDS_BETWEEN_DISP_UPDATE) - 59;             // Case where timer needs to roll over
    }

    else {
        gblinfo.next_disp_update = (gblinfo.tick1000ms + SECONDS_BETWEEN_DISP_UPDATE);
        if(gblinfo.next_disp_update > 59) gblinfo.next_disp_update = SECONDS_BETWEEN_DISP_UPDATE;       // Cath possible concurrency issue here

    }
}

void GetNewDisplayShtdnTime( void ) {
    if((gblinfo.tick1000ms + SECONDS_DISP_BACKLIGHT_ON)) {
        gblinfo.next_disp_shtdn = (gblinfo.tick1000ms + SECONDS_DISP_BACKLIGHT_ON) - 59;             // Case where timer needs to roll over
    }

    else {
        gblinfo.next_disp_shtdn = (gblinfo.tick1000ms + SECONDS_DISP_BACKLIGHT_ON);
        if(gblinfo.next_disp_shtdn > 59) gblinfo.next_disp_shtdn = SECONDS_DISP_BACKLIGHT_ON;       // Cath possible concurrency issue here

    }

}

/* END OF FILE */