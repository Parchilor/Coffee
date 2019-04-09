
#include <stdint.h>

#include "main.h"
#ifdef BOOTLOADER
void System_BootInfo(void)
{
    uint32_t chipuid[3];
    uint16_t flash_size;
	RCC_ClocksTypeDef clock;
	RCC_GetClocksFreq(&clock);
    chipuid[0] = *(__IO uint32_t *)(FLASH_UID_ADDR2); // High Byte
    chipuid[1] = *(__IO uint32_t *)(FLASH_UID_ADDR1); // 
    chipuid[2] = *(__IO uint32_t *)(FLASH_UID_ADDR0); // Low Byte
    flash_size= *(u16*)(FLASH_SIZE_ADDR);
    // hal_debug("%s By %s ", BOOTLOADER_NAME, BOOTLOADER_AUTHOR);
    hal_debug("\r\n\r\nSingle Bootloader By "BOOTLOADER_AUTHOR);
    hal_debug(" ("__DATE__" - "__TIME__")\r\n\r\n\r\n");
    // hal_debug("Chip UID: %X-%X-%X \r\n", chipuid[0], chipuid[1], chipuid[2]);
    hal_debug("CPU  : "STM32_MODEL "("STM32_KERNEL")\r\n");
	hal_debug("Freqs: SYSCLK=%dMHz,HCLK=%dMHz,PCLK1=%dMHz,PCLK2=%dMHz,\r\n", 
				clock.SYSCLK_Frequency/1000/1000,
				clock.HCLK_Frequency/1000/1000,
				clock.PCLK1_Frequency/1000/1000,
				clock.PCLK2_Frequency/1000/1000
				);
    hal_debug("Board: "BOARD_VERSION"\r\n");
	
    hal_debug("Flash: %dKB\r\n", flash_size);
	
    hal_debug("Firmware Version: V%d.%d.%d \r\n",
                    __STM32F4XX_STDPERIPH_VERSION_MAIN,
                    __STM32F4XX_STDPERIPH_VERSION_SUB1,
                    __STM32F4XX_STDPERIPH_VERSION_SUB2);    
	
}

void System_Jump(uint32_t address)
{
    typedef void (*pFunction)(void);

    pFunction Jump_To_Application;

    // variable that will be loaded with the start address of the application
    vu32 *JumpAddress;
    const vu32 *ApplicationAddress = (vu32 *)address;

    // get jump address from application vector table
    JumpAddress = (vu32 *)ApplicationAddress[1];

    // load this address into function pointer
    Jump_To_Application = (pFunction)JumpAddress;

    // Disable all interrupts
    int i;
    for (i = 0; i < 8; i++)
        NVIC->ICER[i] = NVIC->IABR[i];

    // Set interrupt vector table
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_OFFSET_ADDR);

    // Set stack pointer as in application's vector table
    __set_MSP((vu32)(ApplicationAddress[0]));

    // Go
    Jump_To_Application();
}

void System_Bootm(void)
{
  System_Init();
  System_BootInfo();
  hal_debug("Booting To %X\r\n", FLASH_START_ADDR);
  __cli();
  System_Jump(FLASH_START_ADDR);
  
}
#endif

#ifdef APPLICATION
void System_Preproccess(void)
{
  __sti();
}
#endif

void PWR_EnterSTDBYMode(void)
{
	// TIM_Cmd(TIM2, DISABLE);
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
	// TIM_Cmd(TIM14, DISABLE);
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, DISABLE);
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM14, ENABLE);
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM14, DISABLE);

	USART_Cmd(USART1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
	USART_DeInit(USART1);
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

	I2C_Cmd(I2C1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE); 
	ADC_DeInit(ADC1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);
	I2C_DeInit(I2C1);

	I2C_ITConfig(I2C1, I2C_IT_RXI, DISABLE);
	I2C_ITConfig(I2C2, I2C_IT_TCI, DISABLE);
	I2C_ITConfig(I2C1, I2C_IT_STOPI, DISABLE);
	I2C_ITConfig(I2C1, I2C_IT_ADDRI, DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, DISABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, DISABLE);
	// GPIO_ResetBits(GPIOA, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOB, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOC, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOD, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOE, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOF, GPIO_Pin_All);
	// GPIO_ResetBits(GPIOG, GPIO_Pin_All);
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
    
	// RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, DISABLE);
	// RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	// RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, DISABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
	RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOA, DISABLE);

	PWR_EnterSTANDBYMode();
}