
                AAPCS INTERWORK, VFP_COMPATIBLE, RWPI_COMPATIBLE
                PRESERVE8


                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                PUBLIC  __vector_table

                DATA

                IMPORT     NMI_Handler                    ; NMI Handler
                IMPORT     HardFault_Handler              ; Hard Fault Handler
                IMPORT     SVC_Handler                    ; SVCall Handler
                IMPORT     PendSV_Handler                 ; PendSV Handler
                IMPORT     SysTick_Handler                ; SysTick Handler

                ; External Interrupts
                IMPORT     WWDG_IRQHandler                ; Window Watchdog
                IMPORT     RTC_IRQHandler                 ; RTC through EXTI Line
                IMPORT     FLASH_IRQHandler               ; FLASH
                IMPORT     RCC_IRQHandler                 ; RCC
                IMPORT     EXTI0_1_IRQHandler             ; EXTI Line 0 and 1
                IMPORT     EXTI2_3_IRQHandler             ; EXTI Line 2 and 3
                IMPORT     EXTI4_15_IRQHandler            ; EXTI Line 4 to 15
                IMPORT     DMA1_Channel1_IRQHandler       ; DMA1 Channel 1
                IMPORT     DMA1_Channel2_3_IRQHandler     ; DMA1 Channel 2 and Channel 3
                IMPORT     DMA1_Channel4_5_IRQHandler     ; DMA1 Channel 4 and Channel 5
                IMPORT     ADC1_IRQHandler                ; ADC1 
                IMPORT     TIM1_BRK_UP_TRG_COM_IRQHandler ; TIM1 Break, Update, Trigger and Commutation
                IMPORT     TIM1_CC_IRQHandler             ; TIM1 Capture Compare
                IMPORT     TIM3_IRQHandler                ; TIM3
                IMPORT     TIM14_IRQHandler               ; TIM14
                IMPORT     TIM15_IRQHandler               ; TIM15
                IMPORT     TIM16_IRQHandler               ; TIM16
                IMPORT     TIM17_IRQHandler               ; TIM17
                IMPORT     I2C1_IRQHandler                ; I2C1
                IMPORT     I2C2_IRQHandler                ; I2C2
                IMPORT     SPI1_IRQHandler                ; SPI1
                IMPORT     SPI2_IRQHandler                ; SPI2
                IMPORT     USART1_IRQHandler              ; USART1
                IMPORT     USART2_IRQHandler              ; USART2	


__vector_table
		DCD     sfe(CSTACK)                    ; Top of Stack
                DCD     __iar_program_start            ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                ; Window Watchdog
                DCD     0                              ; Reserved
                DCD     RTC_IRQHandler                 ; RTC through EXTI Line
                DCD     FLASH_IRQHandler               ; FLASH
                DCD     RCC_IRQHandler                 ; RCC
                DCD     EXTI0_1_IRQHandler             ; EXTI Line 0 and 1
                DCD     EXTI2_3_IRQHandler             ; EXTI Line 2 and 3
                DCD     EXTI4_15_IRQHandler            ; EXTI Line 4 to 15
                DCD     0                              ; Reserved
                DCD     DMA1_Channel1_IRQHandler       ; DMA1 Channel 1
                DCD     DMA1_Channel2_3_IRQHandler     ; DMA1 Channel 2 and Channel 3
                DCD     DMA1_Channel4_5_IRQHandler     ; DMA1 Channel 4 and Channel 5
                DCD     ADC1_IRQHandler                ; ADC1 
                DCD     TIM1_BRK_UP_TRG_COM_IRQHandler ; TIM1 Break, Update, Trigger and Commutation
                DCD     TIM1_CC_IRQHandler             ; TIM1 Capture Compare
                DCD     0                              ; Reserved
                DCD     TIM3_IRQHandler                ; TIM3
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     TIM14_IRQHandler               ; TIM14
                DCD     TIM15_IRQHandler               ; TIM15
                DCD     TIM16_IRQHandler               ; TIM16
                DCD     TIM17_IRQHandler               ; TIM17
                DCD     I2C1_IRQHandler                ; I2C1
                DCD     I2C2_IRQHandler                ; I2C2
                DCD     SPI1_IRQHandler                ; SPI1
                DCD     SPI2_IRQHandler                ; SPI2
                DCD     USART1_IRQHandler              ; USART1
                DCD     USART2_IRQHandler              ; USART2
                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __vector_table

                 END
