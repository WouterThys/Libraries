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

static uint8_t writeData[3] = {0x01, 0x02, 0x03};
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
    
    uartDriverInit(UART1_BAUD, &onUartReadDone);
    uartDriverEnable(true);
    i2cDriverInit();  
   
    printf("Slave start\n");
    i2cDriverEnable(true);
   
//    masterPackage.address = I2C_ADDRESS;
//    masterPackage.command = 0;
//    masterPackage.data = &writeData[0];
//    masterPackage.length = 1;
//   
//    i2cDriverWrite(&masterPackage);
//    checkI2CStatus(masterPackage);
    
    while (1) {
        LED1 = !LED1;
        
        DelayMs(100);
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
