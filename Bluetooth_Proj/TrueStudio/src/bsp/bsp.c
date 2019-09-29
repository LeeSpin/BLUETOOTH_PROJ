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

void bspInit(void)
{
	uart1_init();
	dcmi_init();
}
