/*
 * led.h
 *
 *  Created on: 2019. 10. 27.
 *      Author: 이진한
 */

#ifndef SRC_HW_CORE_LED_H_
#define SRC_HW_CORE_LED_H_

#include "hw_def.h"

void ledInit();
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);

#endif /* SRC_HW_CORE_LED_H_ */
