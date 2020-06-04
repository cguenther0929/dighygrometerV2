/******************************************************************************
*   FILE: main.c
*
*   PURPOSE: Main source
*
*   TODO:
*
*   NOTE: When reading from timer one in 16bit mode,
*           we must read from the LOW byte first!
*
******************************************************************************/

#include "main.h" //Include header file associated with main.c

// CONFIG1L
#pragma config RETEN        = OFF      // VREG Sleep Enable bit (Disabled - Controlled by SRETEN bit)
#pragma config INTOSCSEL    = HIGH      // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL      = DIG       // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST        = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC         = INTIO1    // Internal RC Oscillator with output on OSC2.  p45/550.
#pragma config PLLCFG       = ON        // PLL x4 Enable bit (Enabled).  Works for internall oscialltor block, 
#pragma config FCMEN        = OFF       // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO         = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN       = ON        // Power Up Timer (Enabled)
#pragma config BOREN        = SBORDIS   // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV         = 1         // Brown-out Reset Voltage bits (2.7V)
#pragma config BORPWR       = ZPBORMV   // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN        = OFF       // Watchdog Timer Disabled in HW
#pragma config WDTPS        = 1024      // Watchdog Postscaler (1:1024)

// CONFIG3L
#pragma config RTCOSC       = SOSCREF   // RTCC Clock Select (RTCC uses SOSC)

// CONFIG3H
#pragma config CCP2MX       = PORTC     // CCP2 Mux (RC1)
#pragma config MSSPMSK      = MSK7      // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE        = ON        // Master Clear Enable (MCLR Enabled, RG5 Disabled)

// CONFIG4L
#pragma config STVREN       = ON        // Stack Overflow Reset (Enabled)
#pragma config BBSIZ        = BB2K      // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0          = OFF       // Code Protect 00800-03FFF (Disabled)
#pragma config CP1          = OFF       // Code Protect 04000-07FFF (Disabled)
#pragma config CP2          = OFF       // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3          = OFF       // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB          = OFF       // Code Protect Boot (Disabled)
#pragma config CPD          = OFF       // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0         = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1         = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2         = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3         = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC         = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB         = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD         = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBRT0        = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBRT1        = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBRT2        = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBRT3        = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBRTB        = OFF      // Table Read Protect Boot (Disabled)

struct GlobalInformation gblinfo;
App_State app_state = STATE_IDLE;
Disp_Actions disp_actions = DISP_TMR_DISABLE;

void main()
{
    SetUp();
    DispInit();

    PrintSplashScreen();

    GetNewDisplayRefreshTime();
    GetNewDisplayShtdnTime();
    
    while (true) {
        if(gblinfo.flag20ms) {
            gblinfo.flag20ms = false;
            EvaluateButtonInputs(); //TODO need to put this back in
            EvaluateState();     //TODO need to put this back in
        }

        if(gblinfo.flag100ms) {
            gblinfo.flag100ms = false;

            if(gblinfo.tick1000ms == gblinfo.next_disp_update){
                GetNewDisplayRefreshTime( );
                app_state       = UPDATE_DISPLAY;        //TODO put this back in
            }
            
            if(gblinfo.tick1000ms == gblinfo.next_disp_shtdn){
                DISP_ENABLE = DISPLAY_OFF;
            }
        
        }

        if(gblinfo.flag500ms) {
            gblinfo.flag100ms = false;
        }

        if(gblinfo.flag1000ms) {
            gblinfo.flag1000ms = false;
            health_led = ~health_led;  
        }

    }

}   //END Main()




void PrintSplashScreen( void ) {

    DispSetContrast(30);
    DispWtLnOne("Digital");
    DispWtLnTwo("Hygrometer");
    tick100msDelay(15);


    DispRefresh();
    DispWtLnOne("FW VERSION:");
    DispLineTwo();
    DispWriteChar(MAJVER + 0x30); DispWriteChar('.');
    DispWriteChar(MINVER + 0x30); DispWriteChar('.');
    DispWriteChar(BUGVER + 0x30);
    tick100msDelay(10);

    DispRefresh();

}

void EvaluateState( void ) {
    uint16_t temp_data;

    switch(app_state) {
        case STATE_IDLE:

            // if(gblinfo.tick20msloopctr >= COUNTS_20MS_BETWEEN_SENSOR_UPDATE) //TODO I think we want to put this back in
            if(gblinfo.tick20msloopctr >= 50){
                app_state = STATE_GRAB_SENSOR_DATA;
                gblinfo.tick20msloopctr = 0;
            }
            else
                gblinfo.tick20msloopctr++;
             
            if(gblinfo.up_btn_pressed) {
                gblinfo.up_btn_pressed = false;
                app_state = STATE_CALIBRATE;
            }

            if(gblinfo.lo_btn_pressed) {
                gblinfo.lo_btn_pressed = false;
                app_state = STATE_CLEAR_CALIBRATE;
            }

            if(gblinfo.both_btns_pressed) {
                gblinfo.both_btns_pressed = false;
                app_state = STATE_MAKE_NETWORK_CONNECTION;
            }

        break;
        
        case STATE_GRAB_SENSOR_DATA:
        
            SelectSensor(SELECT_SENSOR_1);
            tick20msDelay(1);
            temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_HUM_HOLD_MASTER);          // TODO put this line in
            // temp_data = 1200;   //TODO take this line out
            gblinfo.rh_value_1 = (float)((125.0 * temp_data / 65536) - 6 + gblinfo.rh_offset_1);

            temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_TMP_PREV_RH_MEAS);         // TODO put this line in
            // temp_data = 1200;       //TODO take this line out
            gblinfo.temp_value_1 = (uint8_t)((temp_data / 207.1952) - 52.24);                   // Units in deg C

            // SelectSensor(SELECT_SENSOR_2);
            tick20msDelay(1);
            temp_data = 1200;       //TODO remove
            // temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_HUM_HOLD_MASTER);   //TODO put this line in
            gblinfo.rh_value_2 = (float)((125.0 * temp_data / 65536) - 6 + gblinfo.rh_offset_2);

            // temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_TMP_PREV_RH_MEAS);  //TODO put this line back int
            temp_data = 1200;
            gblinfo.temp_value_2 = (uint8_t)((temp_data / 207.1952) - 52.24);             // Units in deg C
        
        break;

        case UPDATE_DISPLAY:
            DISP_ENABLE = DISPLAY_ON;
            
            DispRefresh();      // Puts cursor on line one, too
            DispWriteString("T1:"); PrintDecimalNumber((uint16_t) gblinfo.temp_value_1); DispWriteString("  ");
            DispWriteString("H1:"); PrintDecimalNumber((uint16_t) gblinfo.rh_value_1);
            
            DispLineTwo();
            DispWriteString("T2:"); PrintDecimalNumber((uint16_t) gblinfo.temp_value_2); DispWriteString("  ");
            DispWriteString("H2:"); PrintDecimalNumber((uint16_t) gblinfo.rh_value_2);

            GetNewDisplayShtdnTime( );
            app_state = STATE_IDLE;
        break;

        case STATE_CALIBRATE:

            SelectSensor(SELECT_SENSOR_1);
            tick20msDelay(1);
            temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_HUM_HOLD_MASTER);
            gblinfo.rh_offset_1 = (float)(75 - ((125.0 * temp_data / 65536) - 6));

            SelectSensor(SELECT_SENSOR_2);
            tick20msDelay(1);
            temp_data = I2CRead_16b(SI7020_BASE_ADDRESS, SI7020_MEAS_HUM_HOLD_MASTER);
            gblinfo.rh_offset_2 = (float)(75.0 - ((125.0 * temp_data / 65536) - 6));


            app_state = STATE_IDLE;
            break;

        case STATE_CLEAR_CALIBRATE:
            gblinfo.rh_offset_1 = 0;
            gblinfo.rh_offset_2 = 0;

            app_state = STATE_IDLE;
            break;
        
        case STATE_MAKE_NETWORK_CONNECTION:   //TODO we may need to change the order of things a bit
            
            DispRefresh();
            DispWtLnOne("Net Connect");
            tick20msDelay(50);

            // EspCommCheck();
            
            // DispRefresh();
            // DispWtLnOne("Set Client Mode");
            // tick100msDelay(10);
            // EspApOrClientMode(ESP_CLIENT_MODE);                 //TODO according to the example, this should be "AT+CWMODE=3"  
            // EspApOrClientMode(ESP_AP_AND_CLIENT_MODE);                 //TODO this matches the example //// Can't seem to get IP when this option set 
            // tick20msDelay(1);

            // DisconnectWifiConnection( );                        // Send command "AT+CWQAP" to disconnect from any AP
            // tick20msDelay(5);
            
            // DispRefresh();
            // DispWtLnOne("Reset Module");
            // tick100msDelay(10);
            // ResetEsp( );
            // tick100msDelay(50);

            DispRefresh();
            DispWtLnOne("Join Network");
            tick100msDelay(10);
            JoinNetwork(WIFI_ROUTER_SSID, WIFI_ROUTER_PASSWORD);
            
            GetAssignedIpAddress( );    
            GetLengthIpAddress( );
            
            // SetEspCipmuxMode(ESP_CIPMUX_SINGLE_CONNECTION);         // Mode = 0 for single connection
            SetEspCipmuxMode(ESP_CIPMUX_MULTIPLE_CONNECTION);       // Mode = 1 for multiple connections

            // EspServerMode (ACTION_OPEN_SERVER_SOCKET,"80");
            // tick20msDelay(5);
            
            SendEmail( );

            GetNewDisplayRefreshTime( );

            app_state = UPDATE_DISPLAY;
            break;

        default:
            DispRefresh();
            DispWtLnOne("STATE ERROR");
            break;
    }

}

// void DisplayDwellTmr( uint8_t action ) {

//     switch (action) {
//         case DISP_TMR_RST:
//             gblinfo.disp_tmr_active = true;
//             gblinfo.disp_seconds_ctr = 0;
//             DISP_ENABLE = DISPLAY_ON;

//             break;

//         case DISP_TMR_CNT:

//             if(gblinfo.disp_tmr_active && (gblinfo.disp_seconds_ctr < MAX_DISP_DWELL))
//                 gblinfo.disp_seconds_ctr++;
//             else if (!gblinfo.disp_tmr_active)
//                 gblinfo.disp_seconds_ctr = 0;

//             break;

//         case DISP_TMR_ENABLE:
//             gblinfo.disp_tmr_active = true;
//             gblinfo.disp_seconds_ctr = 0;
//             break;

//         case DISP_TMR_DISABLE:
//             gblinfo.disp_tmr_active = true;
//             gblinfo.disp_seconds_ctr = 0;
//             break;
//     }

// }

void EvaluateButtonInputs ( void ) {
    if(PB1_GPIO == BUTTON_PUSHED && PB2_GPIO == BUTTON_PUSHED) {
        (gblinfo.up_btn_press_ctr > 2)?(gblinfo.up_btn_press_ctr -= 2):(gblinfo.up_btn_press_ctr = 0);
        (gblinfo.lo_btn_press_ctr > 2)?(gblinfo.lo_btn_press_ctr -= 2):(gblinfo.lo_btn_press_ctr = 0);

        gblinfo.btn_both_press_ctr++;
    }
    else if(PB1_GPIO == BUTTON_PUSHED && PB2_GPIO == BUTTON_RELEASED) {
        (gblinfo.lo_btn_press_ctr > 2)?(gblinfo.lo_btn_press_ctr -= 2):(gblinfo.lo_btn_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);

        gblinfo.up_btn_press_ctr++;
    }
    else if(PB1_GPIO == BUTTON_RELEASED && PB2_GPIO == BUTTON_PUSHED) {
        (gblinfo.up_btn_press_ctr > 2)?(gblinfo.up_btn_press_ctr -= 2):(gblinfo.up_btn_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);

        gblinfo.lo_btn_press_ctr++;
    }
    else {
        (gblinfo.up_btn_press_ctr > 2)?(gblinfo.up_btn_press_ctr -= 2):(gblinfo.up_btn_press_ctr = 0);
        (gblinfo.lo_btn_press_ctr > 2)?(gblinfo.lo_btn_press_ctr -= 2):(gblinfo.lo_btn_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);

    }

    if(gblinfo.up_btn_press_ctr >= BUTTON_DEBOUNCE_TICKS) {

        gblinfo.up_btn_press_ctr = 0;
        gblinfo.up_btn_pressed       = true;
        gblinfo.lo_btn_pressed       = false;
        gblinfo.both_btns_pressed    = false;
    }

    else if(gblinfo.lo_btn_press_ctr >= BUTTON_DEBOUNCE_TICKS) {

        gblinfo.lo_btn_press_ctr = 0;
        gblinfo.lo_btn_pressed       = true;
        gblinfo.up_btn_pressed       = false;
        gblinfo.both_btns_pressed    = false;
    }

    else if(gblinfo.btn_both_press_ctr >= BUTTON_DEBOUNCE_TICKS) {

        gblinfo.btn_both_press_ctr = 0;
        gblinfo.both_btns_pressed    = true;
        gblinfo.up_btn_pressed       = false;
        gblinfo.lo_btn_pressed       = false;
    }

}

void SetUp(void)
{
    uint16_t i;
    
    /* Oscillator Frequency Switch */
    PLLEN = 1;
    for(i=0; i<500; i++);
    while(!MFIOFS);             // Watch the internal osciallator stable bit
    for(i=0; i<500; i++);
    
    app_state = STATE_IDLE;

    /* PIN DIRECTION FOR LEDs */
    TRISB5 = output;                    // Output for system health LED.

    /* PIN DIRECTION FOR PUSH BUTTONS */
    TRISB0 = input;                     // Push Button 1 input
    TRISB1 = input;                     // Push Button 2 input

    /* PIN DIRECTION FOR DISPLAY AND EXTERNAL SPI */
    TRISB4 = output;                    // Active low output enables display
    TRISC0 = output;                    // Register select line to Display
    TRISC1 = output;                    // Display reset signal
    TRISC2 = output;                    // Display SPI chip select signal
    TRISC3 = output;                    // Display SPI SCK signal
    TRISC4 = input;                     // Display SPI MISO signal
    TRISC5 = output;                    // Display SPI MOSI signal
    TRISF1 = output;                    // External device CS signal

    /* GPIO SIGNALS TO HEADER */
    TRISE7 = output;                    // Port tied to GPIO 1
    TRISE6 = output;                    // Port tied to GPIO 2
    TRISE5 = output;                    // Port tied to GPIO 3
    TRISE4 = output;                    // Port tied to GPIO 4
    TRISE3 = output;                    // Port tied to GPIO 5
    TRISE2 = output;                    // Port tied to GPIO 6

    /* PIN DIRECTIONS FOR TEMP/HUMIDITY SENSOR */
    TRISD5 = input;                     // I2C SDA2 line -- must be defined as input
    TRISD6 = input;                     // I2C SCL2 line -- must be defined as input
    TRISD7 = output;                    // Output that drive analog switch for selecting between sensor 1 and 2

    /* PIN DIRECTIONS FOR WIFI MODULE */
    TRISA0 = output;                    // Enable line to WiFi module
    TRISA1 = output;                    // Reset line to WiFi module
    TRISC6 = output;                    // Serial transmit line to WiFi module
    TRISC7 = input;                     // Serial receive line from WiFi module

    /* UNUSED PINS DEFINED AS OUTPUTS TO SAVE POWER */  
    TRISA7 = output;
    TRISA3 = output;
    TRISA4 = output;
    TRISA5 = output;

    TRISB2 = output;
    TRISB3 = output;

    TRISG4 = output;
    TRISG3 = output;
    TRISG2 = output;
    TRISG1 = output;
    TRISG0 = output;

    TRISF7 = output;
    TRISF6 = output;
    TRISF5 = output;
    TRISF4 = output;
    TRISF3 = output;
    TRISF2 = output;

    TRISE1 = output;
    TRISE0 = output;

    TRISD4 = output;
    TRISD3 = output;
    TRISD2 = output;
    TRISD1 = output;
    TRISD0 = output;

    /* BUTTON INITIALIZATION */
    gblinfo.lo_btn_press_ctr            = 0;
    gblinfo.up_btn_press_ctr            = 0;
    gblinfo.lo_btn_press_ctr            = 0;
    gblinfo.btn_both_press_ctr          = 0;
    gblinfo.up_btn_pressed              = false;
    gblinfo.lo_btn_pressed              = false;
    gblinfo.both_btns_pressed           = false;

    /* Sensor Initialization */
    gblinfo.rh_value_1                  = 0;
    gblinfo.rh_offset_1                 = 0;
    gblinfo.temp_value_1                = 0;
    gblinfo.rh_value_2                  = 0;
    gblinfo.rh_offset_2                 = 0;
    gblinfo.temp_value_2                = 0;

    /* INITIAL CONDITION OF HEALTH LED */
    health_led = LED_OFF;

    /* TURN OFF THE DISPLAY */
    DISP_ENABLE = DISPLAY_OFF;

    /* WiFi Reset and Enable Lines High */
    WIFI_RESET  = 0;
    WIFI_ENABLE = 1;
    for(i=0; i<5000; i++);
    WIFI_RESET  = 1;


    /* SETUP INTERRUPTS */
    Init_Interrupts();

    /* INITIALIZE SPI INTERFACE FOR DISPLAY */
    SPI1Init();

    //TODO remove?
    // AnalogRefSel(REF2D048, EXTREF);             // Use internal 2.048V reference and External VREF pin for negative reference -- page 216/380
    // InitA2D(RIGHT_JUSTIFIED, 4);                // Set up AD (Justification, Acq Time TADs) ==> (Right, 4 TAD) -- page 361/550

    /* SETUP ANALOG CHANNELS */
    ANCON0 = 0x00;                      // Analog channels 7-0 are configured for digital inputs. p.363
    ANCON1 = 0x00;                      // Analog channel 10-8 are configred for digital inputs. p.364

    gblinfo.tick20ms = 0;
    gblinfo.tick100ms = 0;
    gblinfo.tick500ms = 0;
    gblinfo.tick1000ms = 0;

    /* Enable the UART */
    SetUpUART();
    EnableUart1Interrupts( UART_RX_HIGH_PRIORITY );

    /* Enable I2C Interface */
    I2Cinit( );
    
    
    /* TIMER FOR APPLICATION INTERRUPTS */
    Timer0Init(TMR0_INTUP_SETTING, TMR0_PRESCALER, TMR0_FSOC_DIV_BY_4); //ARGS: interrupts, prescaler, clksource = FOSC/4
    Timer0On();

}

void tick100msDelay(uint16_t ticks)
{
    uint16_t i = 0;
    uint16_t tick = 0; //Used to lock time value
    for (i = ticks; i > 0; i--)
    {
        tick = gblinfo.tick100ms;
        while (tick == gblinfo.tick100ms); //Wait for time to wrap around (in one half tick1000mond)
    }
}

void tick20msDelay(uint16_t ticks)
{
    uint16_t i = 0;
    uint16_t tick = 0; //Used to lock time value
    for (i = ticks; i > 0; i--)
    {
        tick = gblinfo.tick20ms;
        while (tick == gblinfo.tick20ms); //Wait for time to wrap around (in one half tick1000mond)
    }
}

void ClockPulseDelay (uint8_t clk_pulses) {
    uint8_t i;
    for(i=0; i<clk_pulses; i++) {
        NOP();
    }

}
/* END OF FILE */