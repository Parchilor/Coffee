#include "main.h"
/*
 *  PA0		MOTOR
 *  PA4		VBAT
 */
void System_ADCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);

	// ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	// ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	// ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	// ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	// ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_GetCalibrationFactor(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADCAL) == SET);
	
	ADC_Cmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY) != SET);
}

int16_t ADC_ReadChannel(uint32_t idx)
{
	uint32_t channels[] = {ADC_Channel_0, ADC_Channel_4}; 
	unsigned short ret = -1;
	hal_flags flags;
	save_and_cli(flags);
	if(idx < countof(channels)) {
		ADC1->CHSELR = 0;
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY) != SET);
		ADC_ChannelConfig(ADC1, channels[idx], ADC_SampleTime_55_5Cycles);
		ADC_StartOfConversion(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOSEQ) != SET);
		__delay_us(20);
		ret = ADC_GetConversionValue(ADC1);
	}
	restore_flags(flags);
	return ret;
}

static void BubbleSort(int16_t* pData,uint16_t Count)
{
	uint16_t iTemp;
	uint16_t i = 0, j = 0;
	for(i = 0; i < Count; i++)	{
		for(j = 0; j < Count-1; j++)	{
			if (pData[j] > pData[j + 1])	{
				iTemp = pData[j + 1];
				pData[j + 1] = pData[j];
				pData[j] = iTemp;
			}
		}
	}
}

int32_t ADC_ReadValue(uint8_t idx)
{
	uint8_t cnt = 0, i, n = 0;
	int16_t tmp = 0;
    int32_t sum = 0;
	int16_t adc_value[16] = {0};
	if(idx == 1)
	{
		GPIO_SetBits(IO_CTRL(VDET_EN));
		delay_ms(25);
	}
	for(i = 0; i < 16; i++)
	{
		tmp = ADC_ReadChannel(idx);
		if(tmp >= 0)
		{
			adc_value[cnt] = tmp;
			cnt++;
		}
	}
	if(idx == 1)
	{
		GPIO_ResetBits(IO_CTRL(VDET_EN));
	}
	BubbleSort(adc_value, cnt);
	for(i = 1; i < cnt - 1; i++)
	{
		n++;
		sum += adc_value[i];
	}
	if(n <= 0) 
	{
		return -1;
	}
	sum /= n;
#ifdef __DEBUG
	hal_debug("ADC[0]: %d\r\n", sum);
#endif
	return sum;
}