/*
 * peripheral_driver.c
 *
 *  Created on: Feb 26, 2025
 *      Author: arunb
 */

#include "peripheral_driver.h"
/*
 * The USART_1 is connected to APB2
 * Port GPIOA is connected to AHB1
 * PIN PA9 USART_TX
 * PIN PPA10 USART_RX both AF7
 */
void USART1_Init(void) {
	RCC->APB2ENR |= USART1_CLK_EN;

	RCC->AHB1ENR |= GPIOA_CLK_EN;

	GPIOA->MODER |= bit_set(2, 18);
	GPIOA->MODER |= bit_set(2, 20);

	GPIOA->AFR[1] |= bit_set(7, 4);
	GPIOA->AFR[1] |= bit_set(7, 8);

	USART1->BRR = uart_division(PERIPHERAL_CLOCK, UART1_BAUD_RATE);

	USART1->CR1 = CR1_TE;
	USART1->CR1 |= CR1_UART_EN;
}

void USART1_DMA_Init(uint32_t source, uint32_t destination, uint32_t size){
	RCC->AHB1ENR |= DMA2_CLK_EN;
	DMA2_Stream7->CR &= bit_reset(0);
	//Clearing interrupts
	DMA2->HIFCR &= bit_reset(22);
	DMA2->HIFCR &= bit_reset(24);
	DMA2->HIFCR &= bit_reset(25);
	DMA2->HIFCR &= bit_reset(26);
	DMA2->HIFCR &= bit_reset(27);
	//destination
	DMA2_Stream7->PAR = destination;
}
