/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f429xx.h"

void UART_Enable(void);
void sendData(char*);
void receiveData(void);

char data_received[10] = { 0 };

void main(void) {
	RCC->APB2ENR |= (1U << 4); //Enabling UART peripheral

	RCC->AHB1ENR |= (1U << 0); //Enabling GPIOA port
	GPIOA->MODER |= (10U << 18); // Enable Alternate function for GPIOA PIN_9 and PIN_10
	GPIOA->AFR[1] |= (7U << 4);
	GPIOA->AFR[1] |= (7U << 8);

	UART_Enable();

	while (1) {

		receiveData();
		sendData("SUCESS\n");

		for (int i = 0; i >= 100000; i++) {
			// send this for delay;
		}
	}

}

void UART_Enable(void) {
	USART1->CR1 &= ~(1U << 15); 	// Over sampling by 16
	USART1->CR1 &= ~(1U << 12); 	// 8 data bits or word length
	USART1->CR1 &= ~(1U << 10); 	// Disable parity
	USART1->CR1 |= (1U << 3); 		// Transmitter enable
	USART1->CR1 |= (1U << 2);		// Receiver enable
	USART1->BRR = 0x683;  			// setting baud-rate as 9600 for a 16 MHZ clock
	USART1->CR1 |= (1 << 13);		// UART enable
}

void sendData(char* data) {
	while (!(USART1->SR |= (1U << 6))) {
		// Stay inside if the already loaded is not sent
	}
	USART1->DR = data++;
}

void receiveData(void) {
	for (int i = 0; i < 5; i++) {
		while (!(USART1->SR |= (1U << 5))) {

		}
		data_received[i] = USART1->DR;

		if (data_received[i] == 0x10) {
			return;
		}
	}

}

//
////#include "stm32f4xx.h"
//
//void TIM3_PWM_Init(void);
//void GPIO_Init(void);
//void Delay(uint32_t delay);
//void ADC1_Init(void);
//
//
//uint32_t dutyCycle = 0;
//int step = 1;
//
//int main(void) {
//    GPIO_Init();
//    TIM3_PWM_Init();
//
//    while (1) {
//        dutyCycle += step;
//        if (dutyCycle >= 100) {
//            dutyCycle = 0;  // Reverse direction
//        }
//        TIM3->CCR1 = dutyCycle;  // Update duty cycle
//        Delay(50);
//    }
//}
//
//void GPIO_Init(void) {
//	RCC->AHB1ENR 	|= (1U << 0);  // Enable GPIO clock
//    GPIOA->MODER	|= (1U << 13);
//    GPIOA->AFR[0] 	|= (2U << 24);
//}
//
//void TIM3_PWM_Init(void) {
//    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable TIM3 clock
//    // Auto-reload (Period = 1ms)
//    TIM3->PSC = (320 - 1);
//    TIM3->ARR = (1000 - 1);
//
//    // PWM mode 1 on CH1
//    TIM3->CCMR1 |= (6U << TIM_CCMR1_OC1M_Pos);
//    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
//    TIM3->CCER |= TIM_CCER_CC1E;
//
//    TIM3->CR1 |= TIM_CR1_ARPE;
//    TIM3->CR1 |= TIM_CR1_CEN; // Enable Timer
//}
//
//void Delay(uint32_t delay) {
//    for (uint32_t i = 0; i < delay * 1000; i++) {
//    }
//}
//
//void ADC1_Init(void)
//{
//	RCC->APB2ENR 	|= (1U << 8);   // enable ADC 1
//	GPIOA->MODER 	|= (3U << 0);
//
//	ADC1->
//}
//void timerFunction(void)
//{
//	TIM1->CNT	&= ~(15U << 0);
//	TIM1->SR	&= ~(1U << 0);
//	TIM1->CR1	|= (1U << 0);
//}
//
//int main(void)
//{
//
//
//	RCC->AHB1ENR 	|= (1U << 0);
//	GPIOA->MODER	|= (1U << 13);
//	GPIOA->AFR[0] 	|= (2U << 24);
//
//	RCC->APB1ENR 	|= (1U << 1);
//	TIM1->PSC 		= 16000;
//	TIM1->ARR 		= 100;
//	TIM1->CCR1		= 30;
//	TIM1->CCMR1 	&= ~(3U << 0);
//	TIM1->CCMR1     |= (13U << 3);
//
//
//    /* Loop forever */
//	for(;;)
//	{
//		timerFunction();
//		while(!(TIM1->SR & (1U << 0)))
//		{
//
//		}
//	}
//}
