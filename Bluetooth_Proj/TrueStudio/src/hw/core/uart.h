/*
 * uart.h
 *
 *  Created on: 2019. 10. 27.
 *      Author: 이진한
 */

#ifndef SRC_HW_CORE_UART_H_
#define SRC_HW_CORE_UART_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_MAX_CH 1

#include "hw_def.h"

void uartInit(void);
bool uartOpen(uint8_t channel, uint32_t baud);
bool uartClose(uint8_t channel);
uint32_t uartAvailable(uint8_t channel);

void uartFlush(uint8_t channel);
void uartPutch(uint8_t channel, uint8_t ch);
uint8_t uartGetch(uint8_t channel);
int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length);

uint8_t uartRead(uint8_t channel);
int32_t uartPrintf(uint8_t channel, const char *fmt, ...);

#endif /* SRC_HW_CORE_UART_H_ */
