#ifndef I2C_DRIVER_H
#define	I2C_DRIVER_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdbool.h>
#include <stdint.h>

#define I2C_MWRITE          3       /* Master write                           */
#define I2C_MREAD           2       /* Master read                            */
#define I2C_OK              1       /* Message ok                             */
#define I2C_NOK             -1      /* Message not ok                         */
#define I2C_OVERFLOW        -2      /* Overflow                               */
#define I2C_COLLISION       -3      /* Data collision                         */
#define I2C_NO_ADR_ACK      -4      /* No acknowledge on address data         */
#define I2C_NO_DATA_ACK     -5      /* No acknowledge on data                 */
#define I2C_UNEXPECTED_DATA -6      /* Unexpected data                        */
#define I2C_UNEXPECTED_ADR  -7      /* Unexpected address                     */
#define I2C_STILL_BUSY      -8      /* Still busy with previous message       */

#define I2C_SCL_Dir     TRISBbits.TRISB8        /* SCL Direction              */
#define I2C_SDA_Dir     TRISBbits.TRISB9        /* SDA Direction              */

#define I2C_SCL_Odc     ODCBbits.ODCB8          /* Open drain config for SCL  */
#define I2C_SDA_Odc     ODCBbits.ODCB9          /* Open drain config for SDA  */

struct I2C_Data {
    uint8_t address;    /* Address (7-bit) of the slave                       */
    uint8_t command;    /* Command (8-bit) send from M -> S                   */
    int8_t  status;     /* Status of R/W                                      */
    uint8_t data1;      /* First data (8-bit) send from S <-> M               */
    uint8_t data2;      /* Second data (8-bit) send from S <-> M              */
};
typedef struct I2C_Data i2cData_t;

/**
 * 
 */
void i2cInitMaster();

/**
 * 
 * @param address
 * @param onI2cAnswer
 * @param onI2cReadDone
 */
void i2cInitSlave(uint16_t address, void (*onI2cAnswer)(i2cData_t * data), void (*onI2cReadDone)(i2cData_t data));

/**
 * 
 * @param enable 
 */
void i2cEnable(bool enable);

/**
 * 
 */
void i2cReset();

/**
 * Write data to slave.
 * @param data: i2cData_t containing address and data for and from the slave
 */
void i2cMasterWrite(i2cData_t *data);

/**
 * Read data from slave
 * @param data
 */
void i2cMasterRead(i2cData_t *data);

/**
 * Read the last data from master, after the Master has initiated a transaction.
 * @param data: i2cData_t containing data from master and answer from slave.
 */
void i2cSlaveRead(i2cData_t *data);

#endif	

