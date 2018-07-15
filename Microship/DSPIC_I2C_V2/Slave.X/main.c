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
#include "Drivers/I2C_MDriver.h"


/*******************************************************************************
 *          TODO
 ******************************************************************************/
/*
 * MASTER:
 *  - BYTE/WORD wide communication: in case of WORD wide -> always R/W two bytes
 * 
 * SLAVE:
 *  - BYTE/WORD wide communication: in case of WORD wide -> always R/W two bytes
 */

/*******************************************************************************
 *          DEFINES
 ******************************************************************************/

/*******************************************************************************
 *          MACRO FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 *          VARIABLES
 ******************************************************************************/
static i2cPackage_t i2cPackage;
static uint16_t i2cBuffer[3] = {0x0000, 0x0000, 0x0000};
static int8_t i2cError;
static bool i2cHasData;

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onUartReadDone(UartData_t data);

static void onI2CEvent(i2cPackage_t package);
static bool checkI2CStatus(i2cPackage_t package);

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

// NOTE !!!! => only add flag to main here, no code because this could mess with
// the I2C read logic!
void onI2CEvent(i2cPackage_t package) {
    i2cHasData = true;
}

bool checkI2CStatus(i2cPackage_t package) {
    //if (data.status != i2cError) {
        i2cError = package.status;
        if (DEBUG_I2C) {
            switch(i2cError) {
                default: break;
                case I2C_NOK: printf("I2C_NOK\n"); break;
                case I2C_OVERFLOW: printf("I2C_OVERFLOW\n"); break;
                case I2C_COLLISION: printf("I2C_COLLISION\n"); break;
                case I2C_NO_ADR_ACK: printf("I2C_NO_ADR_ACK\n"); break;
                case I2C_NO_DATA_ACK: printf("I2C_NO_DATA_ACK\n"); break;
                case I2C_UNEXPECTED_DATA: printf("I2C_UNEXPECTED_DATA\n"); break;
                case I2C_UNEXPECTED_ADR: printf("I2C_UNEXPECTED_ADR\n"); break;
                case I2C_STILL_BUSY: printf("I2C_STILL_BUSY\n"); break;
            }
        }
    //}
    
    if (i2cError < I2C_OK) {
        //i2cDriverReset();
        return false;
    } else {
        return true; 
    }
}


/*******************************************************************************
 *          MAIN PROGRAM
 ******************************************************************************/
int main(void) {

    initialize();
    DelayMs(100);
    
    i2cPackage.data = i2cBuffer;
    i2cPackage.length = 3;
    
    uartDriverInit(UART1_BAUD, &onUartReadDone);
    uartDriverEnable(true);
    i2cDriverInit(&i2cPackage, &onI2CEvent);  
   
    printf("Slave start\n");
    i2cDriverEnable(true);
    
    while (1) {
        
        if (i2cHasData) {
            i2cHasData = false;
            
            printf("Slave RW:\n");
            
            uint8_t i;
            for (i = 0; i < i2cPackage.length; i++) {
                printf(" %d: %d\n", i, i2cPackage.data[i]);
            }
        }
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
