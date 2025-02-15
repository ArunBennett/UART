/*
 * peripheral_driver.h
 *
 *  Created on: Feb 26, 2025
 *      Author: arunb
 */

#ifndef INC_PERIPHERAL_DRIVER_H_
#define INC_PERIPHERAL_DRIVER_H_

#include "main.h"
#include <stdint.h>
#include "stm32f429xx.h"

#define PERIPHERAL_CLOCK 		16000000U
#define UART1_BAUD_RATE			9600
/*
 * USARTR 1 driver
 */
#define USART1_CLK_EN			(1U << 4)
#define GPIOA_CLK_EN 			(1U << 0)
#define CR1_TE 					(1U << 3)
#define CR1_UART_EN 			(1U << 13)
#define CR1_TXE 				(1U << 7)
// DMA settings for USART1
//USART 1 RX stream2 and channel 4
//USART 1 TX Stream7 and channel 4
#define DMA2_CLK_EN				(1U << 22)
#define DMA_STREAM_EN			(1U << 0)


/*
 * Function declaration
 */
void USART1_Init(void);
void USART1_Write(void);
void USART1_DMA_Init(uint32_t source, uint32_t destination, uint32_t size);

static inline uint32_t bit_set(uint32_t pos, uint32_t shift) {
	return (pos << shift);
}

static inline uint32_t bit_reset(uint32_t shift) {
	return ~(1U << shift);
}

static inline uint16_t uart_division(uint32_t PeripheralClock,
		uint32_t BaudRate) {
	return ((PeripheralClock + (BaudRate / 2U)) / BaudRate);
}
#endif /* INC_PERIPHERAL_DRIVER_H_ */
