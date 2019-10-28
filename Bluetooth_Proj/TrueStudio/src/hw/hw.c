/*
 * hw.c
 *
 *  Created on: 2018. 10. 18.
 *      Author: 이진한
 */


#include "hw.h"

void hwInit(void)
{
  bspInit();

  uartInit();//uartInit();
  ledInit();
}

void delay(uint32_t time_ms)
{
	HAL_Delay(time_ms);
}

uint32_t millis(void)
{
	return HAL_GetTick();
}
