/******************************************************************************
*   FILE: i2c.c
*
*   PURPOSE: Source file which contains all I2C-related routines.  
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

#include "i2c.h"			//Pull in the bus header file

void I2Cinit( void ) {
	/* SET ALL THE CORRECT VALUES IN THE I2C CON REGISTER */
	SMP1 = 1;                   //Slew rate control bit (0=Slew Rate Control enabled for high speed mode; 1=Disabled for standard speed)
    WCOL1 = 0;                  //Write collision detect bit.  Must be cleared in SW.
    SSPOV1 = 0;                 //Receiver overflow indicator bit. Must be cleared in SW.
	SSPCON1bits.SSPM = 8;       //I2C Master Mode. CLOCK RATE = FOSC/(4*(SSPADD+1)) p.293
	SSPEN1 = 1;	                //Enable the I2C module and configure the SDA and SCL Pins as serial port pins
    
    /* LOAD THE BAUD RATE GENERATOR WITH THE APPROPIATE VALUE */
	SSPADD = BaudValue;      //In master-mode, this is the BAUD rate reload value. (p.298) 
}

void I2CWrite_16(uint8_t baseaddress, uint8_t subaddress, uint16_t senddata) {  
	uint8_t tempaddr = 0;		
    uint16_t i = 0;
    uint8_t rtndata = 0;	
    
    WCOL1 = 0;                           //Write collision detect bit.  Must be cleared in SW.
    SSPOV1 = 0;                          //Receiver overflow indicator bit. Must be cleared in SW.
    
    /* WRITE TO I2C ADDRESS OF DEVICE*/
    tempaddr = (uint8_t)((baseaddress << 1) | I2CWRITE);		//LSB is Read/n_Write bit

    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set
        
    SSP1IF = 0;
    I2CGenStart = 1; 			        //Generate the start condition
    while(SSP1IF == 0);                 //Start condition will generate an interrupt 
    if (WCOL1 == 1){                    //Bus collision detected (p.320).  If not collided here, we should own the bus from here on out.
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C51");          //TODO the following is temporary for debugging;
        while(TRUE);        //TODO remove
        return;
    }
    
    SSP1BUF = tempaddr;			//Load the address and r/w bits into the transmit buffer

    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        return;
    }

    SSP1IF = 0;
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){         //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C70"); 
        while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }
    
    /* SEND THE POINTER ADDRESS TO THE CHIP */
    SSP1IF = 0;
    SSP1BUF = subaddress;		        //Send the sub address to the slave

    if (WCOL1 == 1){                    //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        return;
    }
    
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){             //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C91"); 
        while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }
	
	/* SEND UPPER 8 BITS OF DATA TO SLAVE */
    SSP1IF = 0;
    SSP1BUF = (uint8_t)((senddata >> 8) & 0xFF);   //Send upper 8 bits of data to slave 
    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        return;
    }
    
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){        //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C109"); while(TRUE);      //TODO the following is temporary for debugging;    
        return;
    }
    
    /* SEND LOWER 8 BITS OF DATA TO SLAVE */
    SSP1IF = 0;
    SSP1BUF = (uint8_t)(senddata & 0xFF);   //Send upper 8 bits of data to slave 
    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        return;
    }
    
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){        //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C127"); while(TRUE);      //TODO the following is temporary for debugging;     
        return;
    }
    
    
    SSP1IF = 0;
    I2CGenStop = 1;					//Create the STOP condition on the bus
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
}


void I2CWrite_SetPointer(uint8_t baseaddress, uint8_t ptr_address) {  
    uint8_t tempaddr = 0;		
    uint16_t i = 0;
    uint8_t rtndata = 0;		
    
    // rtndata = SSP1BUF;                   //Read from SSP1BUF to "clean"
    // for(i=0;i<i2cdelay;i++);
    WCOL1 = 0;                           //Write collision detect bit.  Must be cleared in SW.
    SSPOV1 = 0;                          //Receiver overflow indicator bit. Must be cleared in SW.
    /* WRITE TO I2C ADDRESS OF DEVICE*/
    tempaddr = (uint8_t)((baseaddress << 1) | I2CWRITE);		//LSP is Read/n_Write bit

    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set
        
    SSP1IF = 0;
    I2CGenStart = 1; 			        //Generate the start condition
    // while(I2CGenStart == 1);            //Bit will automatically get cleared in HW
    while(SSP1IF == 0);                 //Start condition will generate an interrupt 
    if (WCOL1 == 1){                    //Bus collision detected (p.320).  If not collided here, we should own the bus from here on out.
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C162");while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }
    
    SSP1BUF = tempaddr;			//Load the address and r/w bits into the transmit buffer
    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C172"); while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }

    SSP1IF = 0;
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){         //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C181"); while(TRUE);      //TODO the following is temporary for debugging;    
        return;
    }
    
    /* SEND THE POINTER ADDRESS TO THE CHIP */
    SSP1IF = 0;
    SSP1BUF = ptr_address;		        //Send the sub address to the slave
    
    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C195"); while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }
    
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){             //Slave did not acknowledge transmission of base address 
        ClearDisp(); CursorHome(); DispSendString("**I2C203"); while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }

    /* GENERATE THE STOP CONDITION ON THE BUS */
    SSP1IF = 0;
    I2CGenStop = 1;					//Create the STOP condition on the bus
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
}

uint32_t I2CRead(uint8_t baseaddress) {
    uint8_t tempaddr = 0;		//Use this as a way to and the R/W bit with the I2CADDR that can be found in the header file
	uint32_t rtndata = 0;		//This will be the returned 16 bit number
    uint16_t i = 0;
    
    WCOL1 = 0;                           //Write collision detect bit.  Must be cleared in SW.
    SSPOV1 = 0;                          //Receiver overflow indicator bit. Must be cleared in SW.
    
    tempaddr = (uint8_t)((baseaddress << 1) | I2CREAD);		//LSB is Read/n_Write bit
    
    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set
    
    SSP1IF = 0;
    I2CGenStart = 1; 			        //Generate the start condition
    while(SSP1IF == 0);                 //Start condition will generate an interrupt 
    
    if (WCOL1 == 1){                     //Bus collision detected (p.320)
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C238");while(TRUE);      //TODO the following is temporary for debugging;
        return 0;
    }
    
    /* SEND CHIP ADDRESS BASE ADDRESS */
    SSP1IF = 0;
    SSP1BUF = tempaddr;			        // Sending address + read
    
    if (WCOL1 == 1){            //Verify no collision after each write to SSP1BUF (p.317)
        WCOL1=0;
        ClearDisp(); CursorHome(); DispSendString("**I2C247");while(TRUE);      //TODO the following is temporary for debugging;
        return;
    }
    
    while(SSP1IF == 0);		        //Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    if(I2CACKStat == NACK){     //Slave did not acknowledge transmission of base address  //TODO this is where I was getting errors! Pick back up here.  
        ClearDisp(); CursorHome(); DispSendString("**I2C261"); 
        while(TRUE);      //TODO the following is temporary for debugging;
        rtndata = 0x00000000;
        return (rtndata);
    }
    
    /* READ MSB of TEMP DATA */
    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP1IF = 0;
    I2CRecEnable = 1;                   //Enable I2C Receiver
    while(SSP1IF == 0);                 //When all eight bits received RCEN cleared in HW, and BF and SSP1IF automatically set
    
    rtndata = SSP1BUF;               //Should automatically clear BF flag (defined by I2CBF)
    
    I2CACKBit = ACK;                    //Set the ACK/NACK bit to ACK
    SSP1IF = 0;
    I2CGenACK = 1;                      //Assert acknowledge on I2C bus for slave to see
    while(SSP1IF == 0);
    
    /* READ LSB of TEMP DATA */
    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP1IF = 0;
    I2CRecEnable = 1;
    while(SSP1IF == 0);                 //When all eight bits received RCEN cleared in HW, and BF and SSP1IF automatically set
    
    rtndata = ((rtndata << 8) | SSP1BUF);               //Should automatically clear BF flag (defined by I2CBF)
    
    I2CACKBit = ACK;               //Set the ACK/NACK bit to ACK
    SSP1IF = 0;
    I2CGenACK = 1;                  //Assert NACK to indicate slave we are finishing the transaction 
    while(SSP1IF == 0);

    /* READ MSB of HUM DATA */
    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP1IF = 0;
    I2CRecEnable = 1;
    while(SSP1IF == 0);                 //When all eight bits received RCEN cleared in HW, and BF and SSP1IF automatically set
    
    rtndata = ((rtndata << 8) | SSP1BUF);               //Should automatically clear BF flag (defined by I2CBF)
    
    I2CACKBit = ACK;               //Set the ACK/NACK bit to ACK
    SSP1IF = 0;
    I2CGenACK = 1;                  //Assert NACK to indicate slave we are finishing the transaction 
    while(SSP1IF == 0);

    /* READ LSB of HUM DATA */
    while(I2C_Active >= 1);             //Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP1IF = 0;
    I2CRecEnable = 1;
    while(SSP1IF == 0);                 //When all eight bits received RCEN cleared in HW, and BF and SSP1IF automatically set
    
    rtndata = ((rtndata << 8) | SSP1BUF);               //Should automatically clear BF flag (defined by I2CBF)
    
    I2CACKBit = NACK;               //Set the ACK/NACK bit to ACK
    SSP1IF = 0;
    I2CGenACK = 1;                  //Assert NACK to indicate slave we are finishing the transaction 
    while(SSP1IF == 0);

    /* GENERATE STOP CONDITION */
    SSP1IF = 0;
    I2CGenStop = 1;					//Create the STOP condition on the bus
    while(SSP1IF == 0);             //Wait for interrupt flag indicating stop condition transmitted to slave
    
    return (rtndata);               //Return data in form xxxxxx(b9)(b8)(b7)(b6)(b5)(b4)(b3)(b2)(b1)(b0)
}