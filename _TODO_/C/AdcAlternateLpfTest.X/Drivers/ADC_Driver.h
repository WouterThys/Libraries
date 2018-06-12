#ifndef ADC_DRIVER_H
#define	ADC_DRIVER_H

/**
 * 
 */
void adcInit(void (*onAdcReadDone)(uint16_t buffer, uint16_t * data));

/**
 * 
 * @param enable
 */
void adcEnable(bool enable);

#endif	/* ADC_DRIVER_H */

