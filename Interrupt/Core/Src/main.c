/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MFRC522.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE BEGIN PV */
static uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void Interrupt_Init(void);
void SPI1_Init(void);
void transfer_data(void);
void NSS_pinLow(void);
void NSS_pinHigh(void);
void Delay(void);
void SPI1_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t value = 0;
char str1[17]={'\0'};
char str2[17]={'\0'};
char str3[17]={'\0'};
char str4[17]={'\0'};
char tmp_str[65]={'\0'};

uint8_t status, cardstr[MAX_LEN+1];
uint8_t card_data[17];
uint32_t delay_val = 1000; //ms
uint16_t result = 0;
uint8_t UID[5];
// a private key to scramble data writing/reading to/from RFID card:
uint8_t Mx1[7][5]={{0x12,0x45,0xF2,0xA8},{0xB2,0x6C,0x39,0x83},{0x55,0xE5,0xDA,0x18},
		  	  	  	{0x1F,0x09,0xCA,0x75},{0x99,0xA2,0x50,0xEC},{0x2C,0x88,0x7F,0x3D}};
uint8_t SectorKey[7];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	Interrupt_Init();
	SPI1_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  	  for (int i = 0; i < 16; i++)
	  	  {
	  		  cardstr[i] = 0;
	  	  }
	  	  status = 0;
	  	  // Find cards
	  	  status = MFRC522_Request(PICC_REQIDL, cardstr);
	  	  if(status == MI_OK)
	  	  {
	  		  // Anti-collision, return card serial number == 4 bytes
	  		  HAL_Delay(1);
	  		  status = MFRC522_Anticoll(cardstr);
	  		  if(status == MI_OK)
	  		  {
	  			  UID[0] = cardstr[0];
	  			  UID[1] = cardstr[1];
	  			  UID[2] = cardstr[2];
	  			  UID[3] = cardstr[3];
	  			  UID[4] = cardstr[4];
	  		  }
			MFRC522_Halt();
			HAL_Delay(1000);
	  	  }
	  	  else{
	  		  //Waiting For Card
	  		  HAL_Delay(1);
	  	  }
	  }
//		transfer_data();
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RC522_CS_Pin */
  GPIO_InitStruct.Pin = RC522_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RC522_CS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Interrupt_Init(void) {
	RCC->AHB1ENR |= (1U << 0);
	GPIOA->MODER &= ~(3U << 0);

	RCC->APB2ENR |= (1U << 14);
	SYSCFG->EXTICR[0] &= ~(15U << 0);

	EXTI->IMR |= (1U << 0);
	EXTI->RTSR |= (1U << 0);
	EXTI->FTSR |= (1U << 0);

	NVIC->ISER[0] = (1U << 6);

	NVIC->ISER[1] = (1U << 4);

}

void EXTI0_IRQHandler(void) {
	if ((EXTI->PR & (1U << 0)) && (GPIOA->IDR & (1U << 0))) {
		flag = 1;
	} else if ((EXTI->PR & (1U << 0)) && !(GPIOA->IDR & (1U << 0))) {
		flag = 0;
	}
	EXTI->PR = (1U << 0);
}

void SPI1_Init(void) {
	RCC->APB2ENR |= (1U << 12);
	RCC->AHB1ENR |= (1U << 0);

	// GPIO config
	GPIOA->MODER &= ~(3U << 8);
	GPIOA->MODER |= (1U << 8);
//	GPIOA->MODER |= (2U << 8);

	GPIOA->MODER |= (2U << 10);  // PA5
	GPIOA->MODER |= (2U << 12);	// PA6
	GPIOA->MODER |= (2U << 14);	// PA7

//	GPIOA->AFR[0] |= (5U << 16);
	GPIOA->ODR |= (1U << 4);	// PA4 as NSS
	GPIOA->AFR[0] |= (5U << 20);
	GPIOA->AFR[0] |= (5U << 24);
	GPIOA->AFR[0] |= (5U << 28);

	SPI1->CR1 &= ~(1U << 11); // data format 8-bit

	SPI1->CR1 |= (3U << 3);		// Baud rate
	SPI1->CR1 |= (1U << 2);		//Master
	SPI1->CR1 &= ~(3U << 0);	// Mode 0
	SPI1->CR1 &= ~(3U << 8); 	// Clearing the 8th and 9th bit
	SPI1->CR1 |= (1U << 9);
	SPI1->CR1 |= (1U << 8);

	SPI1->CR2 |= (1U << 7);  // TXEIE
//	SPI1->CR1 &= ~(1U << 8);

	SPI1->CR1 |= (1U << 6);
}

void transfer_data(void) {
	NSS_pinLow();// When configured as AF this has no effect simply the ss is enabled
	for (int i = 0; i < 16; i++) {
		while (!(SPI1->SR & (1U << 1))) {

		}
		SPI1->DR = i;
		while (!(SPI1->SR & (1U << 7))) {
		}

		Delay();
	}
	NSS_pinHigh();
}

void NSS_pinLow(void) {
	GPIOA->ODR &= ~(1U << 4);
}

void NSS_pinHigh(void) {
	GPIOA->ODR |= (1U << 4);
}

void Delay(void) {

	for (int i = 0; i < 1000; i++) {
		__asm__("nop");
		//send this for delay;
	}
}

void SPI1_IRQHandler(void)
{
	static int i = 0;
	NSS_pinLow();// When configured as AF this has no effect simply the ss is enabled
//	for (int i = 0; i < 100; i++) {
//
//		SPI1->DR = i;
//	}
	SPI1->DR = i;
	i++;

	if(i >= 1000)
	{
		i = 0;
	}
	NSS_pinHigh();
	EXTI->PR = (1U << 7);
//	SPI1->CR1 &= ~(1U << 6);
}

HAL_StatusTypeDef SPI_Transmit(const uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef status;
	HAL_SPI_Transmit_DMA(&hspi1, pData, Size);

	return status;
}

HAL_StatusTypeDef SPI_Receive(uint8_t *pData, uint16_t Size)
{
	HAL_SPI_Receive_DMA(&hspi1, pData, Size);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
