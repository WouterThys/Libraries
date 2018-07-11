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
static int8_t i2cError;

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onUartReadDone(UartData_t data);

static void onI2CAnswer(i2cPackage_t * package);
static void onI2CReadDone(i2cPackage_t package);
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

/**
 * The I2C master asks for data, this method should fill the data buffer with
 * the data the master requests.
 * @param data
 */
void onI2CAnswer(i2cPackage_t * package) {
    uint16_t v1 = 0x1234;
    uint16_t v2 = 0x4321;
    uint16_t v3 = 0x55AA;
    switch(package->length) {
        case 1:
            split(v1, &package->data[0].data1, &package->data[0].data2);
            break;
        case 2:
            split(v1, &package->data[0].data1, &package->data[0].data2);
            split(v2, &package->data[1].data1, &package->data[1].data2);
            break;
        case 3:
            split(v1, &package->data[0].data1, &package->data[0].data2);
            split(v2, &package->data[1].data1, &package->data[1].data2);
            split(v3, &package->data[2].data1, &package->data[2].data2);
            break;
        default:
            break;
    }
}

/**
 * A full read operation has occurred
 * @param data
 */
void onI2CReadDone(i2cPackage_t package) {
    if (checkI2CStatus(package)) {
        if (DEBUG_I2C) {
            printf("I2C data:\n");
            uint16_t l = 0;
            for (l = 0; l < package.length; l++) {
                uint16_t v;
                concatinate(package.data[l].data1, package.data[l].data2, &v);
                printf(" d%d=%d\n", l, v); 
            }
        }
    }
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
    i2cDriverInitSlave(I2C_ADDRESS, &onI2CAnswer, &onI2CReadDone);    
    
    DelayMs(100);
    if (DEBUG) printf("Slave start\n");
    i2cDriverEnable(true);
    
    while (1) {
        
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
