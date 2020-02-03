/******************************************************************************
*   FILE: isr.c
*
*   PURPOSE: Interrupt service routines are contained in this file.  This file contains
*           algorithms that pertain to the interrupt service routine.
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/

#include "isr.h"                //Include the header file for this module.

struct GlobalInformation gblinfo;
struct UARTMembers  uart;         //Structure for all thing UART related

void Init_Interrupts( void ) {

    IPEN = 1;           //This allows interrupts to be defined a priority (either high or low) -- see page 103/380
    GIEH = 1;           //Enable all interrupts with high priority
    GIEL = 1;           //Enable all interrupts with low priority

}

void PORTBINTSetup( uint8_t channel, bool edge_rising, bool highpri ) {
	switch (channel) {
        case 0: 
            //No option to set priority on this interrupt -- it is always high!
            TRISB0 = input;
            INT0IF = 0;     //Clear this flag before enabling interrupt 
            INT0IE = 1;     //Enable the INT0 interrupt
            (edge_rising == true)?(INTEDG0 = 1):(INTEDG0 = 1);
        break;
        
        case 1:
            TRISB1 = input;
            INT1IF = 0;     //Clear this flag before enabling interrupt 
            INT1IE = 1;     //Enable INT1 interrupt 
            (highpri == true) ? (INT1IP = 1):(INT1IP = 0);
            (edge_rising == true)?(INTEDG1 = 1):(INTEDG1 = 1);
        break;
        
        case 2:
            TRISB2 = input;
            INT2IF = 0;     //Clear this flag before enabling interrupt 
            INT2IE = 1;     //Enable the INT2 interrupt
            (highpri == true) ? (INT2IP = 1):(INT2IP = 0);
            (edge_rising == true)?(INTEDG2 = 1):(INTEDG2 = 1);
        break;

        default:
        
        break;

    }

}

// void interrupt high_priority edges_isr( void ) {     
__interrupt (high_priority) void edges_isr( void ) {     
    
    /* Handle UART interrupt */
    if(RC1IF) {                                     // Flag set when a character is received form the UART
        uart.rxchar = RCREG;
        
        uart.rxbuf[uart.new_byte] = RCREG;          // Load this byte into rx buffer  
            
        uart.data_count += 1;                                                   //Increase data counter
        (uart.new_byte >= MAX_BUFFER) ? (uart.new_byte = 0):(uart.new_byte += 1);       //Increase "producer" index
        
        RC1IF = 0;                                   //Clear the interrupt flag.
    }
    
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
    }
    
    // if(INTCON3bits.INT1IF){
    //     INTCON3bits.INT1IF = 0;     //Clear the interrupt flag
    //     INTCON3bits.INT1IE = 0;     //Clear the interrupt flag
    // }
    
    if(INTCON3bits.INT2IF){
        INTCON3bits.INT2IF = 0;     //Clear the interrupt flag
    }

    if(INTCON3bits.INT3IF) {
        INTCON3bits.INT3IF = 0;     //Clear the interrupt flag
    }

}

// void interrupt low_priority main_isr( void ) {
__interrupt (low_priority) void main_isr( void ) {

    uint8_t temp = 0;
    
    if(TMR0IF){                                     //Timer 1 interrupt
        TMR0IF = 0;                                 //Software is responsible for clearing this flag
        TMR0H = TMR0HIGH;                           //Load the high register for the timer -- looking for 1/100 of a tick1000ms
        TMR0L = TMR0LOW;                            //Load the low register for the timer
        
        gblinfo.flag20ms = true;
        
        if(gblinfo.tick20ms >= 4) {
            gblinfo.tick20ms = 0;               
            gblinfo.flag100ms = true;
            
            if(gblinfo.tick100ms >= 4) {             //500ms increment 
                gblinfo.tick100ms = 0;           
                gblinfo.flag500ms = true;
                
                if(gblinfo.tick500ms >= 1) {         //We've ticked away one second
                    gblinfo.tick500ms = 0;           //Reset seconds counter
                    gblinfo.flag1000ms = true;
                    
                    if(gblinfo.tick1000ms >= 59) 
                        gblinfo.tick1000ms = 0;
                    else
                        gblinfo.tick1000ms++;
                
                }   
                
                else
                    gblinfo.tick500ms += 1;                     //Increment seconds counter
            }
            
            else 
                gblinfo.tick100ms += 1;                         //Increment 100 millisecond timer 
        }
        else 
            gblinfo.tick20ms += 1;                               //Increment 1 millisecond timer
        

    }   /* END IF FOR TMR1IF */
    
    // if(PIR2bits.TMR3IF){
        // PIR2bits.TMR3IF = 0;                     //Clear the interrupt flag  
    // } /* END IF FOR TMR3IF */


    if(INT1IF){
        INTCON3bits.INT1IF = 0;     //Clear the interrupt flag
    }
    
    // if(INTCON3bits.INT2IF){
    //     INTCON3bits.INT2IF = 0;     //Clear the interrupt flag
    // }

    // if(INTCON3bits.INT3IF) {
    //     INTCON3bits.INT3IF = 0;     //Clear the interrupt flag
    // }
    
} /* END void interrupt low_priority main_isr( void ) */

void DisableInterrupts( void ) {
    GIEH = 0;           //Disable high priority interrupts
    GIEL = 0;           //Disable low priority interrupts
}

void EnableInterrupts( void ) {
    GIEH = 1;           //Enable high priority interrupts
    GIEL = 1;           //Enable low priority interrupts
}