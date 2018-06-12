#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <math.h>


#include "Drivers/SYSTEM_Driver.h"

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
    
    while (1) {
        
        DelayMs(500);
        PORTBbits.RB15 = 1;
        PORTBbits.RB14 = 0;
        PORTBbits.RB13 = 0;
        
        DelayMs(500);
        PORTBbits.RB15 = 0;
        PORTBbits.RB14 = 1;
        PORTBbits.RB13 = 0;
        
        DelayMs(500);
        PORTBbits.RB15 = 0;
        PORTBbits.RB14 = 0;
        PORTBbits.RB13 = 1;
        
    }
    return 0;
}