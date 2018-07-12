/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define DEBUG       0           /* General debug enable                       */
#define DEBUG_I2C  (1 & DEBUG)  /* I²C debug enable                           */
#define DEBUG_FSM  (1 & DEBUG)  /* FSM debug enable                           */

/**
 * Interrupt priorities (Highest (7) - Lowest(1))
 */

#define IP_U1RX      6
#define IP_I2C       5
#define IP_MENU_TMR  2
#define IP_MAIN_TMR  1
    
/**
 *  PIC
 */
 
#define LED1            PORTAbits.RA0
#define LED1_Dir        TRISAbits.TRISA0
#define LED2            PORTAbits.RA1
#define LED2_Dir        TRISAbits.TRISA1
#define LED3            PORTBbits.RB0
#define LED3_Dir        TRISBbits.TRISB0
    
/**
 * UART
 */      
#define UART1_BAUD      57600 /* 57600 */
#define UART1_ID        2   

#define UART1_RX_Dir    TRISBbits.TRISB4    /* RX Pin on RP36-RB4             */
#define UART1_TX_Dir    TRISAbits.TRISA4    /* TX Pin on RP20-RA4             */

#define UART1_RX_Map    0b0100100           /* RX on RP36                     */
#define UART1_TX_Map    RPOR0bits.RP20R     /* TX on RP20                     */

/**
 * I²C
 */
#define I2C_ADDRESS     0x03

#define I2C_SCL_Dir     TRISBbits.TRISB6        /* SCL Direction              */
#define I2C_SDA_Dir     TRISBbits.TRISB5        /* SDA Direction              */

#define I2C_SCL_Odc     ODCBbits.ODCB6          /* Open drain config for SCL  */
#define I2C_SDA_Odc     ODCBbits.ODCB5          /* Open drain config for SDA  */

#endif	/* XC_HEADER_TEMPLATE_H */

