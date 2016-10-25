/*
 * functions.h
 *
 *  Created on: 18. 10. 2016
 *      Author: michal1
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include <stddef.h>
#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_adc.h"


/* Private typedef */
/* Private define  */
#define B1 2000
#define B2 2900
#define B3 3400
#define B4 3600

#define B_VYP 3800

#define CLASIC 1
#define FLOATING 0

 uint16_t AD_value;
 int mode;

void adc_init(void);
void initLED();
void blinkingLED(int period);

void ADC1_IRQHandler (void);

void initUSART2();
void PutcUART2(char ch);
void sendValue();
void USART2_IRQHandler(void);
#endif /* VRS_CV5_H_ */
