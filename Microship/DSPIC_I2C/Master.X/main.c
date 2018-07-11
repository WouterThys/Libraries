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
static i2cPackage_t masterPackage;
static int8_t i2cError;

static uint16_t writeData[3] = {0x5A5A, 0x1324, 0x0F0F};
static uint16_t readData[3] = {0x0000, 0x0000, 0x0000};

/*******************************************************************************
 *          LOCAL FUNCTIONS
 ******************************************************************************/
static void initialize();
static void onUartReadDone(UartData_t data);

static void i2cSendData(uint16_t * data, uint16_t length);
static void i2cReadData(uint16_t * data, uint16_t length);
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

void i2cSendData(uint16_t * data, uint16_t length) {
    masterPackage.command = length;
    masterPackage.length = length;
    
    uint16_t l;
    for (l = 0; l < length; l++) {
        split(*(data+l), &masterPackage.data[l].data1, &masterPackage.data[l].data2);
    }
    
    i2cDriverMasterWrite(&masterPackage);
    
    if (checkI2CStatus(masterPackage)) {
        if (DEBUG_I2C) {
            printf("I2C package with length %d send!\n", length);
        }
    }
}

void i2cReadData(uint16_t * data, uint16_t length) {
    masterPackage.command = length;
    masterPackage.length = length;
    
    i2cDriverMasterRead(&masterPackage);
    
    if (checkI2CStatus(masterPackage)) {
        if (DEBUG_I2C) {
            printf("I2C data:\n");
            uint16_t l;
            for (l = 0; l < length; l++) {
                concatinate(masterPackage.data[l].data1, masterPackage.data[l].data2, (data + l));
                printf(" d%d=%d\n", l, *(data + l)); 
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
    i2cDriverInitMaster();
    masterPackage.address = I2C_ADDRESS; // Slave address
    
    DelayMs(100);
    if (DEBUG) printf("Master start\n");
    i2cDriverEnable(true);
    
    DelayMs(2000); // Give slave time to start
    
    // Send 1
    i2cSendData(&writeData[0], 1);
    DelayMs(1);
    
    // Send 2
    i2cSendData(&writeData[0], 2);
    DelayMs(1);
    
    // Send 3
    i2cSendData(&writeData[0], 3);
    DelayMs(1);
    
    // Read 1
    i2cReadData(&readData[0], 1);
    DelayMs(1);
    
    // Read 2
    i2cReadData(&readData[0], 2);
    DelayMs(1);
    
    // Read 3
    i2cReadData(&readData[0], 3);
    DelayMs(1);
    
    
    while (1) {
        
       
    }
    return 0;
}


/*******************************************************************************
 *          INTERRUPTS
 ******************************************************************************/
