#include <xc.h>
#include <p18f2550.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 16000000L

void delay(uint16_t ms);

void main(void) {
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    ADCON1bits.PCFG = 0b1111;
    
    while(1) {
        
        PORTBbits.RB0 = !PORTBbits.RB0;
        
        delay(500);
       
    }
    return;
}

void delay(uint16_t ms) {
    uint16_t i = 0;
    while(ms > 0) {
        ms--;
        __delay_ms(1);
    }
}