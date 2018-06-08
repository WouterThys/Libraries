#ifndef SPI1_DRIVER_H
#define	SPI1_DRIVER_H

#ifndef SPI1_SCK_Pin
    #define SPI1_SCK_Pin    PORTBbits.RB10      /* SCK Pin on RP42-RB10           */
    #define SPI1_SCK_Dir    TRISBbits.TRISB10
    #define SPI1_SCK_Map    0b001001 
#endif

#ifndef SPI1_SDO_Pin
    #define SPI1_SDO_Pin    PORTBbits.RB11      /* SDO Pin on RP43-RB11           */
    #define SPI1_SDO_Dir    TRISBbits.TRISB11  
    #define SPI1_SDO_Map    0b001000 
#endif
  
#ifndef SPI1_SDI_Pin
    #define SPI1_SDI_Pin    PORTBbits.RB12      /* SDI Pin on RPI44-RB12          */
    #define SPI1_SDI_Dir    TRISBbits.TRISB12  
    #define SPI1_SDI_Map    44
#endif

#ifndef SPI1_IP
    #define SPI1_IP         1
#endif

    
extern uint16_t SPI1_ReadData;
    
/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/    
    
/**
 *  Initializes the SPI2 module on with SPI ports defined in settings.h. SPI2
 *  will be a 16-bit module. SPI2 mode is Mode 0,0.
 */
void spi1Init();

/**
 * 
 * @param enable
 */
void spi1Enable(bool enable);

/**
 * 
 * @param data
 */
void spi1Write(uint16_t data);



#endif