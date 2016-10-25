/*
 * functions.c
 *
 *
 *  Created on: 18. 10. 2016
 *      Author: michal1
 */


#include <vrs_cv5.h>

void adc_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);

	ADC_ITConfig(ADC1, ADC_IT_EOC,ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam preru�en� n�jdete v s�bore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);


	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}


void ADC1_IRQHandler (void)
{
	if(ADC1->SR & ADC_SR_EOC){
		AD_value = ADC1->DR;
	}
}

void initLED(){

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	  GPIO_InitTypeDef gpioInit;
	  gpioInit.GPIO_Mode = GPIO_Mode_OUT;
	  gpioInit.GPIO_OType = GPIO_OType_PP;
	  gpioInit.GPIO_Pin = GPIO_Pin_5;
	  gpioInit.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_Init(GPIOA, &gpioInit);
}

void blinkingLED(int period){
	if (period == 0){
     	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
     	return;
	}
	if  (period < 0){
     	GPIO_SetBits(GPIOA, GPIO_Pin_5);
     	return;
	}

	  GPIO_SetBits(GPIOA, GPIO_Pin_5);
for (int i = 0; i < period; i++);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	for (int i = 0; i < period; i++);
}
