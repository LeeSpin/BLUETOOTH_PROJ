/*
 * led.c
 *
 *  Created on: 2019. 10. 27.
 *      Author: 이진한
 */

#include "led.h"


void ledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ledOn(uint8_t ch)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

void ledOff(uint8_t ch)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void ledToggle(uint8_t ch)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
