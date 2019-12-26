/******************************************************************************
*   FILE: dispdriver.c
*
*   PURPOSE: Driver file for 8 bit displays.  Originally written for a 
*           Newhaven display
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
#include "dispdriver.h"

void DispTrigger(void) {
    uint16_t i;
    DISP_E = 0;
    // tick10msDelay(1);
    for(i=0;i<500;i++);     //Temp, replace with delay function
    DISP_E = 1;
}

void DispSendChar(uint8_t bytein, bool command) {
    (command==true)?(DISP_RS = disp_command):(DISP_RS = disp_data);
    DISP_BYTE = bytein;
    DISP_RW = disp_write;
    DispTrigger();
}

void DispSendString(const char * y) {
    while(*y != '\0'){
        DispSendChar(*y,false);
        y++;
    }
}

void CursorHome(void) {
    DispSendChar(0x02,true);
}

void ClearDisp(void) {
    DispSendChar(0x01,true);
}

void PrintUnsignedDecimal (uint8_t number) {
    char temphex[3];            //8 bit number so max of 255 (i.e. three digits)
    uint8_t i;                  //Use this as a counter
    uint8_t decimal_count;      //Number of digits as determined by sprintf

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = sprintf(temphex, "%u", number); //u tells the function we want an unsigned decimal number

    /* PLACE EACH OF THE DIGITS ON THE DISPLAY */
    for(i = 0; i < decimal_count; i++) {    //Place the arry of ASCII characters on the display
        DispSendChar(temphex[i],false);
    }

}

void PrintFloat (float number) {
    char temphex[8];                //Define the array that will hold the ASCII values
    uint8_t i;                      //Use this as a counter
    uint16_t j;
    uint8_t decimal_count; //This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = sprintf(temphex, "%.4f", number);   //f tells the function we want to print a float value

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        DispSendChar(temphex[i],false);
    }
}

void DisplayON( void ){
    TRISE7 = output;        //Active low signal to enable display power
    DISP_PWR_EN_n = 0;      //Turn the display on
    DisplayInit();
}

void DisplayOFF( void ){
     
    /* TURN THESE TO HI-Z */
    TRISD  = 0xFF;          //The entire port shall be an output  
    
    TRISE0 = input;         //The entire port can be an output
    TRISE1 = input;         //The entire port can be an output
    TRISE2 = input;         //The entire port can be an output

    DISP_PWR_EN_n = 1;      //Remove power from display
    TRISE7 = input;        //Make Hi-Z -- pull up resistor will take care of things from here.  
}

void DisplayInit(void) {

    /* PIN DIRECTIONS FOR LCD */
    TRISD = output; //The entire port shall be an output 
    TRISE = output; //The entire port can be an output.
    tick10msDelay(1);
    
    DispSendChar(0x30,true);            //Send command 0x30 = Wake up
    tick10msDelay(1);                     //must wait 5ms, busy flag not available
    
    DispSendChar(0x30,true);            //Send command 0x30 = Wake up #2
    tick10msDelay(1);             
    
    DispSendChar(0x30,true);            //Send command 0x30 = Wake up #3
    tick10msDelay(1); 

    DispSendChar(0x30,true);        //Function set: [0,0,1,DL] [N,F,0,0] :  DL = 1|0 = 8 | 4 bit interface ; N = 1|0 = 2 | 1 Lines ; F = 5x11 | 5x8 Font
    
    DispSendChar(0x10,true);        //Set cursor : [0,0,0,1] [S/C,R/L,0,0] : S/C = 1|0 = ? : R/L = 1|0 = ? 
    
    DispSendChar(0x0C,true);        //Display ON/OFF Ctrl : [0,0,0,0] [1,D,C,B] : D=1|0=Disp ON | Disp OFF : C=1|0=Cursor ON|OFF : B=1|0= Blink ON|OFF : 0b0000 1100
    
    DispSendChar(0x06,true);        //Entry mode set : [0,0,0,0][0,1,I/D,SH] : I/D=1|0=? : SH=1|0=?
}