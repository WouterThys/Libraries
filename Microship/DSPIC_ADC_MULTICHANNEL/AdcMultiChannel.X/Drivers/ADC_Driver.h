#ifndef ADC_DRIVER_H
#define	ADC_DRIVER_H

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

void adcDriverInit(void (*onAdcReadDone)(uint16_t buffer, uint16_t * data));
void adcDriverEnable(bool enable);


#endif