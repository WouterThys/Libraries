
/*
Timer 3 is setup to time-out every 200 microseconds (5Khz Rate). 
As a result, the module will stop sampling and trigger a A/D conversion on every Timer3 time-out, i.e., Ts=200us. 

ADC is configured in 10bit mode to alternatively sample AN2/AN3 analog input on Timer 3 interrupt. 
It will take TWO Timer3 Timeout period to sample AN2 first and then AN3.

ADC module clock time period is configured as Tad=Tcy*(ADCS+1)= (1/70M)*140 = 2.00us (500Khz). 
Hence the conversion time for 10-bit A/D Conversion Time Tc=12*Tad = 24.0us

DMA is used to sort and transfer the converted data to DMA RAM. DMA is configured in ping-pong mode 
and it transfers 16 samples of each of the TWO analog inputs and generates interrupt. 

DMA channel 0 is configured in ping-pong mode to move the converted data from ADC to DMA RAM 
on every sample/convert sequence. 
First, DMA uses DMA0STA base address to store the ADC samples and it generates interrupt 
after transferring (TWO x 16 samples = 32 samples).
Next, DMA uses DMA0STB base address to store the ADC samples and it generates interrupt
after transfer (TWO x 16 samples = 32 samples).
Above process repeats continuously. 

void __attribute__((__interrupt__)) _DMA0Interrupt(void);
DMA interrupt service routine, moves the data from DMA buffer to ADC signal buffer 
 
 */




#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <math.h>


#include "Drivers/SYSTEM_Driver.h"
#include "Drivers/ADC_Driver.h"

/*******************************************************************************
 *          DEFINES
 ******************************************************************************/


/*******************************************************************************
 *          MACRO FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 *          VARIABLES
 ******************************************************************************/
uint16_t adcValues[4] = {0,0,0,0};

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onReadDone(uint16_t buffer, uint16_t * data);
static void initAdc1();
static void initAdc2();

void initialize() {
    sysInterruptEnable(false);

    // Initialize system
    sysInitPll();
    sysInitOscillator();
    sysInitPorts();

    // Interrupts
    sysInitInterrupts();
    sysInterruptEnable(true);
}

/*******************************************************************************
 *          MAIN PROGRAM
 ******************************************************************************/
int main(void) {

    initialize();
    DelayMs(100);
    
    
    adcInit(&onReadDone);
    
//    initAdc2();
    
    while (1) {
        
//        while(!_AD1IF);
//        _AD1IF = 0;
//        adcValues[0] = ADC1BUF0;
//        adcValues[1] = ADC1BUF1;
//        adcValues[2] = ADC1BUF2;
//        adcValues[3] = ADC1BUF3;
        
    }
    return 0;
}


/**
 * Fixed multiple input selection example. Converts:
 *  - AN0 -> ADC1BUF3
 *  - AN1 -> ADC1BUF0
 *  - AN2 -> ADC1BUF1
 *  - AN3 -> ADC1BUF2
 * 
 * In the main while(1) loop:
 *   DelayUs(100);
 *   AD1CON1bits.SAMP = 0; // Start conversion
 *   while(!_AD1IF);
 *   _AD1IF = 0;
 *   adcValues[0] = ADC1BUF0;
 *   adcValues[1] = ADC1BUF1;
 *   adcValues[2] = ADC1BUF2;
 *   adcValues[3] = ADC1BUF3;
 */
void initAdc1() {
    // Ports
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA1 = 1;
    TRISAbits.TRISA1 = 1;
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB1 = 1;
    TRISBbits.TRISB1 = 1;
    
    // ADC
    AD1CON1bits.ADON = 0;
    AD1CON1bits.ASAM = 1; // Sampling begins immediately after the last conversion
    AD1CON1bits.SIMSAM = 1;  //Samples CH0, CH1, CH2, CH3 simultaneously (when CHPS<1:0> = 1x); or samples CH0 and CH1 simultaneously (when CHPS<1:0> = 01
    
    AD1CON2bits.CHPS = 0b11; // Converts CH0, CH1, CH2, CH3
    
    AD1CON3bits.ADRC = 0;       // ADC Clock is derived from Systems Clock
    AD1CON3bits.ADCS = 139;     // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/70M)*140 = 2.0µs (500kHz)
    
    AD1CON4bits.ADDMAEN = 0; // No DMA
    AD1CON4bits.DMABL = 0; // Allocates 1 word or buffer to each analog input
    
    AD1CSSH = 0x0000;   // No channel scan
    AD1CSSL = 0x0000;
    
    AD1CHS0bits.CH0SA = 3; // Channel 0 positive input is AN3
    AD1CHS0bits.CH0NA = 0; // VRef- for negative input
    
    AD1CHS123bits.CH123SA = 0; // AN0, AN1, AN2
    AD1CHS123bits.CH123NA = 0; // VRef- for negative input
    
    AD1CON1bits.ADON = 1;
    DelayUs(20);

}

/**
 * Scan four analog inputs using CH0. Converts:
 *  - AN0 -> ADC1BUF3
 *  - AN1 -> ADC1BUF0
 *  - AN2 -> ADC1BUF1
 *  - AN3 -> ADC1BUF2
 * 
 * In the main while(1) loop:
 *   DelayUs(100);
 *   AD1CON1bits.SAMP = 0; // Start conversion
 *   while(!_AD1IF);
 *   _AD1IF = 0;
 *   adcValues[0] = ADC1BUF0;
 *   adcValues[1] = ADC1BUF1;
 *   adcValues[2] = ADC1BUF2;
 *   adcValues[3] = ADC1BUF3;
 */
void initAdc2() {
    // Ports
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA1 = 1;
    TRISAbits.TRISA1 = 1;
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB1 = 1;
    TRISBbits.TRISB1 = 1;
    
    // ADC
    AD1CON1bits.ADON = 0;
    AD1CON1bits.AD12B = 1; // 12-bit mode
    AD1CON1bits.ASAM = 1; // Sampling begins immediately after the last conversion
    AD1CON1bits.SIMSAM = 0;  // Not multiple channels
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 0b111; // Internal counter ends sample and starts conversion
    
    AD1CON2bits.SMPI = 3; // Increment rate is 3 
    AD1CON2bits.CHPS = 0; // Converts CH0
    AD1CON2bits.CSCNA = 1; // Scans inputs for CH0+ during Sample MUXA
    
    AD1CON3 = 0x1F1F;
    AD1CON4 = 0x0000;
    
    AD1CSSH = 0x0000;   // No channel scan
    AD1CSSL = 0x000F;  // AN0, AN1, AN2, AN3
    
    AD1CHS0bits.CH0SA = 0; // Channel 0 positive input is AN3
    AD1CHS0bits.CH0NA = 0; // VRef- for negative input
    
    AD1CON1bits.ADON = 1;
    DelayUs(20);

}







uint16_t seq = 0;
uint16_t buffer1[64];
uint16_t buffer2[64];
uint16_t buffer3[64];
uint16_t buffer4[64];

void onReadDone(uint16_t buffer, uint16_t * data) {
    
    int i = 0;
    
    switch (buffer) {
        case 0:
            for (i = 0; i < 8; i++) {
                buffer1[i + (seq * 8)] = data[i];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++) {
                buffer2[i + (seq * 8)] = data[i];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                buffer3[i + (seq * 8)] = data[i];
            }
            break;
        case 3:
            for (i = 0; i < 8; i++) {
                buffer4[i + (seq * 8)] = data[i];
            }
            seq++;
            break;
    }
    
    if ((seq * 8) >= 64) {
        seq = 0;
    }
}