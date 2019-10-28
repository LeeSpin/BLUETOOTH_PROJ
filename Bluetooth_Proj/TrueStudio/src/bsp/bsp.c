/*
 * bsp.c
 *
 *  Created on: 2018. 10. 18.
 *      Author: �씠吏꾪븳
 */

#include "bsp.h"
#include "stm32f4xx_hal.h"

//UART_InitTypeDef huart1;
UART_HandleTypeDef huart1;
DCMI_HandleTypeDef hdcmi;
I2C_HandleTypeDef SCCB1;

/*
void SCCB_Init(void)
{
	I2C_Init();

	SCCB1.Instance = I2C1;
	SCCB1.Init.AddressingMode;
	SCCB1.Init.ClockSpeed;
	SCCB1.Init.DualAddressMode;
	SCCB1.Init.DutyCycle;
	SCCB1.Init.GeneralCallMode;
	SCCB1.Init.NoStretchMode;
	SCCB1.Init.OwnAddress1;
	SCCB1.Init.OwnAddress2;
}
*/

void DMA_Init(void)
{


}

void uart1_init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate 		= 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart1);

}

void dcmi_init(void)
{
	  hdcmi.Instance = DCMI;
	  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_EMBEDDED;
	  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_FALLING;
	  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
	  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
	  hdcmi.Init.SyncroCode.FrameEndCode = 0;
	  hdcmi.Init.SyncroCode.FrameStartCode = 0;
	  hdcmi.Init.SyncroCode.LineStartCode = 0;
	  hdcmi.Init.SyncroCode.LineEndCode = 0;
	  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
	  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
	  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
	  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
	  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
	  HAL_DCMI_Init(&hdcmi);
}


void SystemClock_Config(void);


void bspInit(void)
{
	HAL_Init();
	SystemClock_Config();

	//__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

void SystemClock_Config()
{

	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;

	    /**Configure the main internal regulator output voltage
	    */
	  __HAL_RCC_PWR_CLK_ENABLE();

	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = 16;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	  RCC_OscInitStruct.PLL.PLLQ = 2;
	  RCC_OscInitStruct.PLL.PLLR = 2;
	  
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    //_Error_Handler(__FILE__, __LINE__);
		while(1);
	  }

	    /**Initializes the CPU, AHB and APB busses clocks
	    */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    //_Error_Handler(__FILE__, __LINE__);
		while(1);
	  }

	    /**Configure the Systick interrupt time
	    */

	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	    /**Configure the Systick
	    */
	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
