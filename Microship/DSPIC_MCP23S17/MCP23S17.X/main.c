#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <math.h>

#include "utils.h"
#include "Settings.h"
#include "Drivers/SYSTEM_Driver.h"
#include "Drivers/ENC_Driver.h"
#include "Controllers/MCP_Controller.h"



/*******************************************************************************
 *          DEFINES
 ******************************************************************************/


/*******************************************************************************
 *          MACRO FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 *          VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void timerEnable(bool enable);

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

void timerEnable(bool enable) {
    T4CONbits.TON = 0; // Disable
    T4CONbits.TCS = 0; // Internal clock (Fp)
    T4CONbits.T32 = 0; // 16-bit timer
    T4CONbits.TCKPS = 0b11; // 1:256 -> 273.437,5 Hz (3.66 µs)
    
    // Registers
    TMR4 = 0x0000;
    if (DEBUG) {
        PR4 = 100;
    } else {
        PR4 = 70; // 3.66µs * 5468 = 20ms
    }
    
    // Interrupts
    _T4IP = IP_MAIN_TMR; 
    _T4IF = 0; // Clear
    _T4IE = 1; // Enable
    
    if (enable) {
        T4CONbits.TON = 1; // Enable
    } 
}

/*******************************************************************************
 *          MAIN PROGRAM
 ******************************************************************************/
int main(void) {

    initialize();
    DelayMs(100);
    
    //encDriverInit();
    mcpInit();
    DelayMs(100);
    
    DelayMs(100);
    //timerEnable(true);
    
    uint8_t cnt = 0;
    
    while (1) {
        
        uint8_t b = mcpGetPORTB();
        
        if ((b & 0x20) > 0) {
            LED1 = 1;
        } else {
            LED1 = 0;
        }
        
        mcpSetPORTA(cnt);
        cnt++;
        DelayMs(2);
        
        uint8_t a = mcpGetPORTA();
        if ((a & 0x01) > 0) {
            LED2 = 1;
        } else {
            LED2 = 0;
        }
        
        DelayMs(2);
    }
    return 0;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _T4Interrupt(void) {
    if (_T4IF) {
        
        LED1 = !LED1;
        
        uint8_t portB = mcpGetPORTB();
        
        if (portB & 0x20) {
            LED2 = 1;
        } else {
            LED2 = 0;
        }
        
        _T4IF = 0; // Clear interrupt
    }
}