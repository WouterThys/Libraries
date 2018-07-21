#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>

#include "utils.h"
#include "Settings.h"

#include "Drivers/SYSTEM_Driver.h"
#include "Drivers/UART_Driver.h"
#include "Drivers/ADC_Driver.h"


/*******************************************************************************
 *          DEFINES
 ******************************************************************************/

/*******************************************************************************
 *          MACRO FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 *          DEFINES
 ******************************************************************************/



/*******************************************************************************
 *          VARIABLES
 ******************************************************************************/
static uint16_t adcData[4];
static volatile bool adcDone = false;
static volatile bool printed = false;

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onAdcDone(uint16_t buffer, uint16_t * data);
static void onUartDone(UartData_t data);

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
    
    // Variables
    uartDriverInit(UART1_BAUD, &onUartDone);
    adcDriverInit(&onAdcDone);
    DelayMs(10);
    
    // Enable
    uartDriverEnable(true);
    
    DelayMs(10);
    printf("start\n");
    DelayMs(10);
    
    adcDriverEnable(true);
    
    while(1) {
      
       
       if (adcDone && !printed)   {
           adcDone = false;
           printed = true;
           printf("ADC done\n");
           
           uint16_t i;
           for (i = 0; i < 4; i++) {
               printf(" %d:%d\n", i, adcData[i]);
           }
       }     
       
    }
}

void onAdcDone(uint16_t buffer, uint16_t* data) {
    
    adcData[buffer] = (uint16_t) average(data, 8);
    adcDone = true;
    
    
}

void onUartDone(UartData_t data) {
    
}


