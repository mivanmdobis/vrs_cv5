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


void initUSART2(){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitTypeDef GPIO_usrt;

	GPIO_usrt.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_usrt.GPIO_Mode = GPIO_Mode_AF;
	GPIO_usrt.GPIO_OType = GPIO_OType_PP;
	GPIO_usrt.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_usrt.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA,&GPIO_usrt);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	//interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	  /* Enable the USARTx Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void PutcUART2(char ch){
	USART_SendData(USART2, (uint8_t) ch);
	//while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		char znak = USART_ReceiveData(USART2);

		if (znak == 'm')
			if (mode == FLOATING)
				mode = CLASIC;
			else mode = FLOATING;

    }else if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);

		if (retazec[counter] != '\0'){
			PutcUART2(retazec[counter]);
			counter++;
		}
    }
}

void sendValue(){

	double AD_valueTmp;

	if (mode == CLASIC){

		sprintf(retazec, "%d", AD_value);
		retazec[4] = 13;
		retazec[5] = '\0';


	}else{

		AD_valueTmp = AD_value;
		AD_valueTmp = AD_valueTmp / 4096*3300.0;

		if (AD_valueTmp < 100)
			sprintf(retazec, "00%d", (int)AD_valueTmp);
		else if (AD_valueTmp < 1000)
			sprintf(retazec, "0%d",(int) AD_valueTmp);
		else
			sprintf(retazec, "%d", (int)AD_valueTmp);

		sprintf(&retazec[2], "%d", (int)AD_valueTmp * 100);
		retazec[6] = '\0';
		retazec[5] = 13;
		retazec[4] = 'V';
		retazec[1] = '.';


	}

	PutcUART2(retazec[0]);
	counter = 1;

	//sleep
	for (int i = 0; i < 50000; i++);
}

