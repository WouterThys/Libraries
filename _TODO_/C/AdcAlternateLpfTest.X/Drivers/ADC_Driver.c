#include <xc.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>
//#include <p33EP32MC202.h>       /* Includes true/false definition                  */

#include "ADC_Driver.h"

/*******************************************************************************
 *          DEFINES
 ******************************************************************************/
#define BUFFER_CHANNELS 4
#define BUFFER_SIZE     32

/*******************************************************************************
 *          MACRO FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 *          VARIABLES
 ******************************************************************************/
unsigned int BufferA[BUFFER_SIZE] __attribute__( (space(ymemory)) );
unsigned int BufferB[BUFFER_SIZE] __attribute__( (space(ymemory)) );

uint16_t dmaBuffer = 0;

/*******************************************************************************
 *          BASIC FUNCTIONS
 ******************************************************************************/
static void (*readDone)(uint16_t buffer, uint16_t * data);

static void initAdc1();
static void initTimer3();
static void initDma0();

void initAdc1() {

    // AD1CON1 register -> configuration
    AD1CON1bits.ADON = 0;
    AD1CON1bits.AD12B = 1;      // 12-bit mode
    AD1CON1bits.ASAM = 1;       // Sampling begins immediately after the last conversion
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 2;       // Timer3 compare ends sampling and starts conversion
    AD1CON1bits.ADDMABM = 0;    // Scatter/Gather mode
    
    // AD1CON2 register -> addressing
    AD1CON2bits.ALTS = 0;       // Always uses channel input selects for Sample MUXA
    AD1CON2bits.SMPI = 3;       // Increment rate is 3, 4 inputs are converted
    AD1CON2bits.CHPS = 0;       // Converts CH0
    AD1CON2bits.CSCNA = 1;      // Scans inputs for CH0+ during Sample MUXA
    
    // AD1CON3 register -> timing
    AD1CON3bits.ADRC = 0;       // System clock
    AD1CON3bits.ADCS = 139;     // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/70M)*140 = 2µs (500Khz)
            
    // AD1CON4 register -> DMA
    AD1CON4bits.ADDMAEN = 1;    // Conversion results are stored in the ADC1BUF0 register for transfer to RAM using DMA
    AD1CON4bits.DMABL = 3;      // Allocates 8 words of buffer to each analog input
    
    // Channel scan select
    AD1CSSH = 0x0000;           // No channel scan
    AD1CSSL = 0x000F;           // AN0, AN1, AN2, AN3
    
    // Interrupts
    _AD1IF = 0;              // Clear flag
    _AD1IE = 0;              // No interrupts
    
    AD1CON1bits.ADON = 1;
}

// Time out every 200µs 
void initTimer3() {
    
    T3CONbits.TCKPS = 1;    // 1:8 -> 8.75 MHz
    PR3 = 1750;             // 1/8.75MHz * 1750 = 200µs
    
    TMR3 = 0x0000;          // Clear register
    _T3IF = 0;              // Clear flag
    _T3IE = 0;              // No interrupts

    //Start Timer 3
    T3CONbits.TON = 1;
}

void initDma0() {
    // DMA0CON registers
    DMA0CONbits.AMODE = 2;  // Configure DMA for Peripheral indirect mode (together with ADC Scatter/Gather)
    DMA0CONbits.MODE = 2;   // Configure DMA for Continuous Ping-Pong mode
    
    // Tie it to ADC1
    DMA0PAD = (volatile uint16_t) &ADC1BUF0;
    DMA0CNT = 31;           // 32 DMA requests (4 buffers with each 8 words)
    DMA0REQ = 13;           // ADC1 as DMA request source
    
    DMA0STAL = (volatile uint16_t) &BufferA;
    DMA0STAH = 0x0000;

    DMA0STBL = (volatile uint16_t) &BufferB;
    DMA0STBH = 0x0000;
   
    // Interrupts
    _DMA0IF = 0;    // Clear the DMA interrupt flag bit
    _DMA0IE = 1;    // Set the DMA interrupt enable bit
    
    // Enable
    DMA0CONbits.CHEN = 1;   // Enable DMA0
}


/*******************************************************************************
 *          DRIVER FUNCTIONS
 ******************************************************************************/
void adcInit(void (*onAdcReadDone)(uint16_t buffer, uint16_t * data)) {
    
    readDone = onAdcReadDone;
    
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA1 = 1;
    TRISAbits.TRISA1 = 1;
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB1 = 1;
    TRISBbits.TRISB1 = 1;
    
    initAdc1();
    initDma0();
    initTimer3();
    
}

void adcEnable(bool enable) {
    
}
/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
void __attribute__ ( (interrupt, auto_psv) ) _DMA0Interrupt( void ) {
    
    if( dmaBuffer == 0 ) {
        PORTBbits.RB15 = !PORTBbits.RB15;
//        (*readDone)(0, &BufferA[0]);
//        (*readDone)(1, &BufferA[8]);
//        (*readDone)(2, &BufferA[16]);
//        (*readDone)(3, &BufferA[24]);
    } else {
        PORTBbits.RB15 = !PORTBbits.RB15;
//        (*readDone)(0, &BufferB[0]);
//        (*readDone)(1, &BufferB[8]);
//        (*readDone)(2, &BufferB[16]);
//        (*readDone)(3, &BufferB[24]);
    }

    dmaBuffer ^= 1;     // Toggle
    _DMA0IF = 0;        //Clear the DMA0 Interrupt Flag
}

void __attribute__ ( (interrupt, auto_psv) ) _T3Interrupt( void ) {
    //PORTBbits.RB15 = !PORTBbits.RB15;
    _T3IF = 0;
}

void __attribute__ ( (interrupt, auto_psv) ) _AD1Interrupt( void ) {
    //PORTBbits.RB15 = !PORTBbits.RB15;
    _AD1IF = 0;
}