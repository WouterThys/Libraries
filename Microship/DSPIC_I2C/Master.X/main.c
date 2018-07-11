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
#include "Drivers/UART_Driver.h"
#include "Drivers/I2C_Driver.h"
#include "../Common/COM_Settings.h"


/*******************************************************************************
 *          TODO
 ******************************************************************************/


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
static void onUartReadDone(UartData_t data);

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

void onUartReadDone(UartData_t data) {
    
}

/*******************************************************************************
 *          MAIN PROGRAM
 ******************************************************************************/
int main(void) {

    initialize();
    DelayMs(100);
    
    uartDriverInit(UART1_BAUD, &onUartReadDone);
   
    DelayMs(100);
    
    // Initial values
    
    
    DelayMs(100);
    if (DEBUG) printf("Slave start\n");
    
    while (1) {
        
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
