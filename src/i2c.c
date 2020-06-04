/******************************************************************************
*   FILE: i2c.c
*
*   PURPOSE: Source file for all I2C-related routines 
*
*   TODO:  
*
*   NOTE: 
*
******************************************************************************/

#include "i2c.h"			//Pull in the bus header file

void I2Cinit( void ) {
	/* SET ALL THE CORRECT VALUES IN THE I2C CON REGISTER */
	SMP2 = 1;                   // Slew rate control bit (0=Slew Rate Control enabled for high speed mode; 1=Disabled for standard speed)
    WCOL2 = 0;                  // Write collision detect bit.  Must be cleared in SW.
    SSPOV2 = 0;                 // Receiver overflow indicator bit. Must be cleared in SW.
	SSP2CON1bits.SSPM = 8;      // I2C Master Mode. CLOCK RATE = FOSC/(4*(SSPADD+1)) p.293
	SSPEN2 = 1;	                // Enable the I2C module and configure the SDA and SCL Pins as serial port pins
    RCEN2 = 0;                  // Start with the receiver disabled
    
    /* LOAD THE BAUD RATE GENERATOR WITH THE APPROPIATE VALUE */
	SSP2ADD = BaudValue;        // SSP1ADD or SSP2ADD p326.  In master mode, lower seven bits used for Baud rate
}


/* 
 * The following describes the mesage format 
 * when needing to write to the user registers
 * 
 * There are a total of two user registers 
 *  1) User Register 1
 *  2) Heater Control Register
 * 
 * The key for the following diagram is as follows
 * 
 *    S: Start
 *    W: Write bit in address is set
 *    A: Acknowledge
 *    Sr: Repeated start condition 
 *    R: Read bit in address is set
 *    NAK: Not acknowledge
 *    P: Stop condition
 * 
 *    +---+ +-------+ +---+ +---+ +----------+
 *    | S +->Slave  +-> W +-> A +-> Register |
 *    +---+ |Address| +---+ +---+ | Command  |
 *          +-------+             +----+-----+
 *                                     |
 *      +------------------------------+
 *      |
 *    +-v-+ +-------+ +---+ +---+
 *    | A +-> Write +-> A +-> P |
 *    +---+ | Data  | +---+ +---+
 *          +-------+
 */    

void I2CWrite_8b(uint8_t baseaddress, uint8_t command, uint8_t data) {  
    uint8_t tempaddr = 0;		
    
    WCOL2 = 0;                          // Write collision detect bit.  Must be cleared in SW.
    SSPOV2 = 0;                         // Receiver overflow indicator bit. Must be cleared in SW.
    while(I2C_Active >= 1);             // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set

    /*
     * Generate start condition
     */ 
    SSP2IF = 0;
    SEN2 = 1; 			                                           
    while(SSP2IF == 0);   

    if (WCOL2){                                                // Bus collision detected (p.320)
        WCOL2=0;
        DispWtLnOne("I2C ERR 1001");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /* 
     * Send Slave Addres 
     * for register to write to
     */   

    tempaddr = (uint8_t)((baseaddress << 1) | I2C_WRITE_BIT);

    SSP2IF = 0;
    SSP2BUF = tempaddr;

    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 1002");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master

    /*
     * Process ACK vs NACK
     * after transmitting address of 
     * command register
     */    
    if(ACKSTAT2 == NACK){     //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 1003");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /*
     * Transmit command to the
     * sensor
     */    
    SSP2IF = 0;
    SSP2BUF = command;			                                    // Transmit command to sensor 
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 1004");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master

    /*
     * Process ACK vs NACK
     * after transmitting command  
     * to sensor 
     */    
    if(ACKSTAT2 == NACK){                   //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 1005");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /*
     * Transmit data 
     * byte to sensor
     */    
    SSP2IF = 0;
    SSP2BUF = data;			                                    // Transmit command to sensor 
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 1006");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master

    /*
     * Process ACK vs NACK
     * after sending data  
     * to sensor 
     */    
    if(ACKSTAT2 == NACK){     //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 1007");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /*
     * This I2C transaction is complete
     * so generate a stop condition on the bus 
     */
    SSP2IF = 0;
    RCEN2 = 1;					            // Create the STOP condition on the bus
    while(SSP2IF == 0);                     // Wait for interrupt flag indicating stop condition transmitted to slave
}  

/*
 * The following function is responsible 
 * for reading from a user register (8b) of the 
 * SI 7020 sensor. For more information, see page
 * 22 in the datasheet
 * 
 * This function breaks convention in the sense
 * that it's very specific to the SI 7020 sensor,
 * however, due to structure of how data is returned,
 * it's easier if the function is specific to the 
 * SI 7020 part
 * 
 * The key for the following diagram is as follows
 * 
 * S: Start
 * W: Write bit in address is set
 * A: Acknowledge
 * Sr: Repeated start condition 
 * R: Read bit in address is set
 * NAK: Not acknowledge
 * P: Stop condition
 *
 * +---+ +-------+ +---+ +---+ +----------+
 * | S +->Slave  +-> W +-> A +-> Read Reg |
 * +---+ |Address| +---+ +---+ | Command  |
 *       +-------+             +----+-----+
 *                                  |
 *   +------------------------------+
 *   |
 * +-v-+ +----+ +---------+ +---+ +---+
 * | A +-> Sr +-> Slave   +-> R +-> A |
 * +---+ +----+ | Address | +---+ +-+-+
 *              +---------+         |
 *      +---------------------------+
 *      |
 *    +-v----+ +-----+ +---+
 *    | Read +-> NAK +-> P |
 *    | Data | +-----+ +---+
 *    +------+
 *   
 */     

uint8_t I2CRead_8b(uint8_t baseaddress, uint8_t command) {
    
    uint8_t tempaddr    = 0x00;		
    uint8_t data_byte   = 0x00;
    
    WCOL2 = 0;                                                      // Write collision detect bit.  Must be cleared in SW.
    SSPOV2 = 0;                                                     // Receiver overflow indicator bit. Must be cleared in SW.
    while(I2C_Active >= 1);                                         // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set
    
    /*
     * Generate start condition 
     */ 
    SSP2IF = 0;
    SEN2 = 1; 			                                            // Generate the start condition
    while(SSP2IF == 0);                                             // Wait for start condition to generate an interrupt
    
    if (WCOL2){                                                // Bus collision detected (p.320)
        WCOL2=0;
        DispWtLnOne("I2C ERR 2001");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    /* 
     * Send Slave Addres 
     * for register to write to
     */
    tempaddr = (uint8_t)((baseaddress << 1) | I2C_WRITE_BIT);		// LSB is Read/n_Write bit
    
    SSP2IF = 0;
    SSP2BUF = tempaddr;			                                    // Sending address + write
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 2002");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master

    /*
     * Process ACK vs NACK
     * after transmitting address of 
     * command register
     */    
    if(ACKSTAT2 == NACK){     //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 2003");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /*
     * Transmit read reg 
     * command to the sensor
     */    
    SSP2IF = 0;
    SSP2BUF = command;			                                    // Transmit command to sensor 
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 2004");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    /*
     * Process ACK vs NACK
     * after transmitting read
     * register command 
     */    
    if(ACKSTAT2 == NACK){     //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 2005");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    /*
     * Create the repeated start condition
     */
    SSP2IF = 0;
    RSEN2 = 1; 			                                            // Generate the start condition
    while(SSP2IF == 0);                                             // Wait for repeated start condition to generate an interrupt
    
    if (WCOL2){                                                // Bus collision detected (p.320)
        WCOL2=0;
        DispWtLnOne("I2C ERR 2006");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    /* 
     * Again, send slave address
     * but now the read bit is set 
     */
    tempaddr = (uint8_t)((baseaddress << 1) | I2C_READ_BIT);		
    
    SSP2IF = 0;
    SSP2BUF = tempaddr;			                                    // Sending address + read
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 2007");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    while(!SSP2IF);		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
    
    /*
     * Process ACK vs NACK
     * after transmitting address of 
     * command register
     */    
    if(ACKSTAT2 == NACK){                   // Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 2008");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /* 
     * Read data byte 
     */
    while(I2C_Active >= 1);             // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP2IF = 0;
    RCEN2 = 1;                          // Enable I2C Receiver
    while(SSP2IF == 0);                 // When all eight bits received RCEN cleared in HW, and BF and SSP2IF automatically set
    
    data_byte = SSP2BUF;                  // Should automatically clear BF flag (defined by I2CBF)
    
    /*
     * Data received, but 
     * protocol dictates 
     * a NACK be sent 
     */
    ACKDT2 = NACK;                       // Set the ACK/NACK bit to ACK
    SSP2IF = 0;
    ACKEN2 = 1;                         // Assert acknowledge on I2C bus for slave to see
    while(SSP2IF == 0);
    
    /*
     * This I2C transaction is complete
     * so generate a stop condition on the bus 
     */
    SSP2IF = 0;
    RCEN2 = 1;					            // Create the STOP condition on the bus
    while(SSP2IF == 0);                     // Wait for interrupt flag indicating stop condition transmitted to slave
    
    return (data_byte);                       
}  
        
/*
 * The following function is responsible 
 * for collecting temperature and humidity
 * data from the SI 7020 sensor.
 * 
 * This function breaks convention in the sense
 * that it's very specific to the SI 7020 sensor,
 * however, due to structure of how data is returned,
 * it's easier if the function is specific to the 
 * SI 7020 part
 * 
 * The key for the following diagram is as follows
 * 
 * S: Start
 * W: Write bit in address is set
 * A: Acknowledge
 * Sr: Repeated start condition 
 * R: Read bit in address is set
 * NAK: Not acknowledge
 * P: Stop condition
 *
 * 
 *       +---+ +-------+ +---+ +---+ +---------+
 *       | S +->Slave  +-> W +-> A +-> Measure |
 *       +---+ |Address| +---+ +---+ | Command |
 *             +-------+             +----+----+
 *                                        |
 *         +------------------------------+
 *         |
 *       +-v-+ +----+ +---------+ +---+ +---+
 *       | A +-> Sr +-> Slave   +-> R +-> A |
 *       +---+ +----+ | Address | +---+ +-+-+
 *                    +---------+         |
 *            +---------------------------+
 *            |
 *       +----v----+ +---------+ +---+ +---------+
 *       | Clock   +-> MS Byte +-> A +-> LS Byte |
 *       | Stretch | +---------+ +---+ +----+----+
 *       +---------+                        |
 *          +-----------OR------------------+
 *          |                               |
 *       +--v--+ +---+                      v
 *       | NAK +-> P |               Optional Checksum
 *       +-----+ +---+               Collection
 *                                   Datasheet p20/35
 *       
 *   
 */     

uint16_t I2CRead_16b(uint8_t baseaddress, uint8_t command) {
    
    uint8_t     tempaddr            = 0;		
    uint8_t     ms_byte             = 0;
    uint8_t     ls_byte             = 0;
    uint32_t    timeout_ticks       = 0;
    bool        i2c_timeout_error   = false;
    
    WCOL2 = 0;                                                      // Write collision detect bit.  Must be cleared in SW.
    SSPOV2 = 0;                                                     // Receiver overflow indicator bit. Must be cleared in SW.

    timeout_ticks = 0;
    while(I2C_Active >= 1 && timeout_ticks < I2C_TIMEOUT_TICKS) {                                         // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 435");
        while(true);
    }    
    
    /*
     * Generate start condition 
     */ 
    SSP2IF = 0;
    SEN2 = 1; 			                                            // Generate the start condition
    
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {       // Wait for start condition to generate an interrupt
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }                                             
    
    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 454");
        while(true);
    }
    
    if (WCOL2){                                                // Bus collision detected (p.320)
        WCOL2=0;
        DispWtLnOne("I2C ERR 458");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    /* 
     * Send Slave Addres 
     * for register to write to
     */
    tempaddr = (uint8_t)((baseaddress << 1) | I2C_WRITE_BIT);		// LSB is Read/n_Write bit
    
    SSP2IF = 0;
    SSP2BUF = tempaddr;			                                    // Sending address + write
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 474");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    timeout_ticks = 0;
    while(!SSP2IF && timeout_ticks < I2C_TIMEOUT_TICKS) {		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 486");
        while(true);
    }
    SSPOV2 = 0;                                                     
    WCOL2=0;

    /*
     * Process ACK vs NACK
     * after transmitting address of 
     * command register
     */    
    if(ACKSTAT2 == NACK){     //Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 496");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /*
     * Transmit command to the
     * sensor
     */    
    SSP2IF = 0;
    SSP2BUF = command;			                                    // Transmit command to sensor 
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 510");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 516");
        while(true);
    }

    SSPOV2 = 0;                                                     
    WCOL2=0;
    
    /*
     * Create the repeated start condition
     */
    SSP2IF = 0;
    RSEN2 = 1; 			                                            // Generate the start condition
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {       // Wait for start condition to generate an interrupt
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }
    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 537");
        tick100msDelay(3);
    }  
    
    if (WCOL2){                                                // Bus collision detected (p.320)
        WCOL2=0;
        DispWtLnOne("I2C ERR 543");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }
    
    SSPOV2 = 0;                                                     
    WCOL2=0;
    
    /* 
     * Again, send slave address
     * but now the read bit is set 
     */
    tempaddr = (uint8_t)((baseaddress << 1) | I2C_READ_BIT);		
    
    SSP2IF = 0;
    SSP2BUF = tempaddr;			                                    // Sending address + read
    
    if (WCOL2){                                                // Verify no collision after each write to SSP2BUF (p.317)
        WCOL2=0;
        DispWtLnOne("I2C ERR 559");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    timeout_ticks = 0;
    while(!SSP2IF && timeout_ticks < I2C_TIMEOUT_TICKS) {		                    // Interrupt shall be created on 9th clock cycle after ACK/NACK bit received by the master
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 571");
        while(true);
    }
    
    SSPOV2 = 0;                                                     
    WCOL2=0;
    
    /*
     * Process ACK vs NACK
     * after transmitting address of 
     * command register
     */    
    if(ACKSTAT2 == NACK){                   // Slave did not acknowledge transmission of base address
        DispWtLnOne("I2C ERR 581");
        DispWtLnTwo("INFINITE LOOP");
        while(true);
    }

    /* 
     * Read most significant
     * byte of data 
     */
    while(I2C_Active >= 1);             // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP2IF = 0;
    RCEN2 = 1;                          // Enable I2C Receiver
    
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }
    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 593");
        tick100msDelay(3);
    } 
    
    ms_byte = SSP2BUF;                  // Should automatically clear BF flag (defined by I2CBF)
    
    SSPOV2 = 0;                                                     
    WCOL2=0;
    
    /*
     * Data received, now set ACK
     * back to slave
     */
    ACKDT2 = ACK;                       // Set the ACK/NACK bit to ACK
    SSP2IF = 0;
    ACKEN2 = 1;                         // Assert acknowledge on I2C bus for slave to see
    
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 614");
        while(true);
    }
    
    /* 
     * Read least significant
     * byte of data 
     */
    while(I2C_Active >= 1);             // Checking if R_nW | ACKEN | RCEN | PEN | RSEN | SEN is set.  RCEN ignored if I2C "active" when we try to set
    
    SSP2IF = 0;
    RCEN2 = 1;                          // Enable I2C Receiver
    
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS){                  // When all eight bits received RCEN cleared in HW, and BF and SSP2IF automatically set
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }
    
    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 632");
        while(true);
    }
    
    ls_byte = SSP2BUF;                  // Should automatically clear BF flag (defined by I2CBF)
    
    SSPOV2 = 0;                                                     
    WCOL2=0;
    
    /*
     * All of data now received,
     * so transmit NACK 
     */
    ACKDT2 = NACK;                               // Send NACK to end transmittion -- SI7020 slave won't send CRC
    SSP2IF = 0;
    ACKEN2 = 1;                                  //Assert NACK to indicate slave we are finishing the transaction 
    
    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 655");
        while(true);
    }
    
    /*
     * This I2C transaction is complete
     * so generate a stop condition on the bus 
     */
    SSP2IF = 0;
    RCEN2 = 1;					            // Create the STOP condition on the bus

    timeout_ticks = 0;
    while(SSP2IF == 0 && timeout_ticks < I2C_TIMEOUT_TICKS) {                     // Wait for interrupt flag indicating stop condition transmitted to slave
        timeout_ticks++;
        timeout_ticks == I2C_TIMEOUT_TICKS ? (i2c_timeout_error = true):(i2c_timeout_error = false);
    }

    if(i2c_timeout_error){
        DispWtLnOne("I2C TMOUT 671s");
        while(true);
    }
    
    return ((ms_byte << 8) | (ls_byte));                      
}    