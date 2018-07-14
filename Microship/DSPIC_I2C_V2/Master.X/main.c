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
 *  - Data buffer and pointer should be given in the i2cDriverInit(..) method
 *  - Event (function pointer) when I²C transaction is done (also in i2cDriverInit(..))
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
static i2cPackage_t masterPackage;
static int8_t i2cError;

static uint8_t writeData[3] = {0x0F, 0x0E, 0x0D};
static uint8_t readData[3] = {0x00, 0x00, 0x00};

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onUartReadDone(UartData_t data);
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
    LED1 = 1;
    
    uartDriverInit(UART1_BAUD, &onUartReadDone);
    uartDriverEnable(true);
    i2cDriverInit();   
    DelayMs(100);
    
    printf("Master start\n");
    i2cDriverEnable(true);
    DelayMs(100);
   
    masterPackage.address = I2C_ADDRESS;
   
    LED1 = 0;
    masterPackage.command = 0; // Address in slave device
    masterPackage.data = &writeData[0];
    masterPackage.length = 3;
    i2cDriverWrite(&masterPackage);
    checkI2CStatus(masterPackage);
    DelayUs(10);
    
    
    LED1 = 0;
    masterPackage.command = 0;
    masterPackage.data = &readData[0];
    masterPackage.length = 3;
    i2cDriverRead(&masterPackage);
    checkI2CStatus(masterPackage);
    DelayUs(10);
    
    DelayMs(100);
    
    uint16_t i;
    printf("Read buffer after writing:\n");
    for (i = 0; i < 3; i++) {
        printf(" %d: %d\n", i, readData[i]);
    }
    
    while (1) {
        LED1 = !LED1;
        
        DelayMs(1000);
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
