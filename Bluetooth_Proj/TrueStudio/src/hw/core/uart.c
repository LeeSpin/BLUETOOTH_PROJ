/*
 * uart.c
 *
 *  Created on: 2019. 10. 27.
 *      Author: 이진한
 */

#include <stdarg.h>

#include "uart.h"
#include "core/qbuffer.h"
//#include "stm32f4xx_hal.h"

#define UART_MODE_POLLING	0
#define UART_MODE_INTERRUPT	1
#define UART_MODE_DMA		2

#define UART_RX_BUF_LENGTH	16
#define UART_RX_QBUF_LENGTH	256

typedef struct
{
	bool is_open;
	uint32_t baud;
	uint8_t	tx_mode;
	uint8_t rx_mode;

	uint8_t	rx_buf[UART_RX_BUF_LENGTH];
	qbuffer_t qbuffer_rx;

	UART_HandleTypeDef handle;
}uart_t;

uart_t uart_tbl[UART_MAX_CH];
uint8_t uart_rx_qbuf[UART_MAX_CH][UART_RX_QBUF_LENGTH];

static void uartStartRx(uint8_t channel);
static void uartRxHandler(uint8_t channel);

void uartInit(void){
	uint8_t i;

	for(i=0; i < UART_MAX_CH ; i++)
	{
		uart_tbl[i].is_open = false;
		uart_tbl[i].rx_mode = UART_MODE_POLLING;
		uart_tbl[i].tx_mode = UART_MODE_POLLING;
	}
}

bool uartOpen(uint8_t channel, uint32_t baud)
{
	bool ret = false;
	uart_t *p_uart;

	p_uart = &uart_tbl[channel];
#if 0
	switch(channel)
	{
		case _DEF_UART1:
			p_uart -> handle.Instance = USART2;
			break;
		case _DEF_UART2:
			p_uart -> handle.Instance = USART2;
		case _DEF_UART3:
			p_uart -> handle.Instance = USART2;
		case _DEF_UART4:
			p_uart -> handle.Instance = USART2;

	}
#endif
	if(channel == _DEF_UART1) p_uart -> handle.Instance = USART2;
	else if(channel == _DEF_UART2) p_uart -> handle.Instance = USART3;
	else if(channel == _DEF_UART3) p_uart -> handle.Instance = UART4;
	else if(channel == _DEF_UART4) p_uart -> handle.Instance = UART5;

	p_uart -> handle.Init.BaudRate 		= baud;
	p_uart -> handle.Init.WordLength 	= UART_WORDLENGTH_8B;
	p_uart -> handle.Init.StopBits 		= UART_STOPBITS_1;
	p_uart -> handle.Init.Parity 		= UART_PARITY_NONE;
	p_uart -> handle.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	p_uart -> handle.Init.Mode			= UART_MODE_TX_RX;


	HAL_UART_DeInit(&p_uart->handle);
	HAL_UART_Init(&p_uart->handle);
#if 0
	if(HAL_UART_DeInit(&p_uart->handle) != HAL_OK)
	{
		break;
	}
	if(HAL_UART_Init(&p_uart->handle) != HAL_OK)
	{
		break;
	}
#endif
	qbufferCreate(&p_uart->qbuffer_rx, uart_rx_qbuf[channel], UART_RX_BUF_LENGTH);

	p_uart->rx_mode = UART_MODE_INTERRUPT;
	p_uart->baud	= baud;
	p_uart->is_open	= true;

	uartStartRx(channel);


	return ret;
}

bool uartClose(uint8_t channel)
{
	bool ret = false;

	if(channel >= UART_MAX_CH)
	{
		return false;
	}

	if(uart_tbl[channel].is_open == true)
	{
		if(HAL_UART_DeInit(&uart_tbl[channel].handle)== HAL_OK)
		{
			ret = true;
		}
	}
	return ret;
}

void uartStartRx(uint8_t channel)
{
	uart_t *p_uart = &uart_tbl[channel];

	if(p_uart->rx_mode == UART_MODE_INTERRUPT)
	{
		HAL_UART_Receive_IT(&p_uart->handle, p_uart->rx_buf, 1);
	}

}

uint32_t uartAvailable(uint8_t channel)
{
	if(channel > UART_MAX_CH)
	{
		return 0;
	}
	return qbufferAvailable(&uart_tbl[channel].qbuffer_rx);
}


void uartFlush(uint8_t channel)
{
	if(uart_tbl[channel].rx_mode == UART_MODE_INTERRUPT)
	{
		qbufferFlush(&uart_tbl[channel].qbuffer_rx);
	}
}

void uartPutch(uint8_t channel, uint8_t ch)
{
	uartWrite(channel, &ch, 1);
}

uint8_t uartGetch(uint8_t channel)
{
	uint8_t ret = 0;

	while(1)
	{
		if(uartAvailable(channel) > 0)
		{
			ret = uartRead(channel);
			break;
		}
	}

	return ret;
}


int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
	int32_t ret = 0;
	uart_t *p_uart = &uart_tbl[channel];

	  if (p_uart->rx_mode == UART_MODE_INTERRUPT)
	  {
	    if (HAL_UART_Transmit(&p_uart->handle, (uint8_t*)p_data, length, 1000) == HAL_OK)
	    {
	      ret = length;
	    }
	  }

	return ret;
}

uint8_t uartRead(uint8_t channel)
{
	uint8_t ret =0;
	uart_t *p_uart = &uart_tbl[channel];

	if(p_uart->rx_mode == UART_MODE_INTERRUPT)
	{
		qbufferRead(&p_uart->qbuffer_rx, &ret, 1);
	}
	return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{

	int32_t ret = 0;
	va_list arg;
	va_start(arg, fmt);
	int32_t len;
	char print_buffer[256];

	len = vsnprintf(print_buffer, 255 , fmt, arg);
	va_end(arg);

	ret = uartWrite(channel, (uint8_t*)print_buffer, len);

	return ret;
}

void uartRxHandler(uint8_t channel)
{
	uart_t *p_uart = &uart_tbl[channel];

	if(p_uart->rx_mode == UART_MODE_INTERRUPT)
	{
		qbufferWrite(&p_uart->qbuffer_rx, &p_uart->rx_buf[0], 1);

		__HAL_UNLOCK(&p_uart->handle);
		uartStartRx(channel);

	}
}

void uartErrHandler(uint8_t channel)
{
	uartFlush(channel);
	uartStartRx(channel);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance == USART2)
	{
		uartRxHandler(_DEF_UART1);
	}
	else if(UartHandle->Instance == USART3)
	{
		uartRxHandler(_DEF_UART2);
	}
	else if(UartHandle->Instance == UART4)
	{
		uartRxHandler(_DEF_UART3);
	}
	else if(UartHandle->Instance == UART5)
	{
		uartRxHandler(_DEF_UART4);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	HAL_UART_DeInit(UartHandle);
	HAL_UART_Init(UartHandle);

	if(UartHandle->Instance == USART2)
	{
		uartErrHandler(_DEF_UART1);
	}
	else if(UartHandle->Instance == USART3)
	{
		uartErrHandler(_DEF_UART2);
	}
	else if(UartHandle->Instance == UART4)
	{
		uartErrHandler(_DEF_UART3);
	}
	else if(UartHandle->Instance == UART5)
	{
		uartErrHandler(_DEF_UART4);
	}
}

void USART2_IRQHandler(void)//not use
{
	HAL_UART_IRQHandler(&uart_tbl[_DEF_UART1].handle);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(huart->Instance == USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();

		GPIO_InitStruct.Pin		= 	GPIO_PIN_2;
		GPIO_InitStruct.Mode	=	GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	=	GPIO_PULLUP;
		GPIO_InitStruct.Speed	=	GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USART2_IRQn, 0 , 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}

}

void HAL_UART_MspDeinit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
}
