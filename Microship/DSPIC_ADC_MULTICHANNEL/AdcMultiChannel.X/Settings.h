// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define DEBUG       1
    
    
/**
 * Interrupt priorities
 */

#define MI2C_IP     7
#define IP_U1RX     6
#define SPI2_IP     5
#define T3_IP       4    
#define DMA1_IP     6
#define IP_CN       1

    
/**
 *  PIC
 */
 
#define LED1            PORTBbits.RB15
#define LED1_Dir        TRISBbits.TRISB15
#define LED2            PORTBbits.RB14
#define LED2_Dir        TRISBbits.TRISB14

/**
 *  ADC
 */
#define ADC_VREF        3.3
#define ADC_n           12
    
#define ADC_BUFFER_SIZE 8
#define ADC_CHANNELS    4       
  
#define ADC0_Asl        ANSELAbits.ANSA0        
#define ADC0_Pin        PORTAbits.RA0
#define ADC0_Dir        TRISAbits.TRISA0
#define ADC0_PNr        1 /* AN0 */
  
#define ADC1_Asl        ANSELAbits.ANSA1           
#define ADC1_Pin        PORTAbits.RA1
#define ADC1_Dir        TRISAbits.TRISA1
#define ADC1_PNr        2 /* AN1 */

#define ADC2_Asl        ANSELBbits.ANSB0      
#define ADC2_Pin        PORTBbits.RB0
#define ADC2_Dir        TRISBbits.TRISB0
#define ADC2_PNr        3 /* AN2 */

#define ADC3_Asl        ANSELBbits.ANSB1       
#define ADC3_Pin        PORTBbits.RB1
#define ADC3_Dir        TRISBbits.TRISB1
#define ADC3_PNr        4 /* AN3 */    
    
    
/**
 * UART
 */    
// Orange = RX
// Geel = TX
#define UART1_BAUD      57600 /* 57600 */
#define UART1_ID        2   

#define UART1_RX_Dir    TRISBbits.TRISB4    /* RX Pin on RP36-RB4             */
#define UART1_TX_Dir    TRISAbits.TRISA4    /* TX Pin on RP20-RA4             */

#define UART1_RX_Map    0b0100100           /* RX on RP36                     */
#define UART1_TX_Map    RPOR0bits.RP20R     /* TX on RP20                     */
    
    

#endif	/* XC_HEADER_TEMPLATE_H */

