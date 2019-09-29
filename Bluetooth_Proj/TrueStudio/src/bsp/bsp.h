/*
 * bsp.h
 *
 *  Created on: 2018. 10. 18.
 *      Author: 이진한
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

void dcmi_init();
void uart1_init();
void SCCB_Init(void);
void bspInit(void);

#endif /* SRC_BSP_BSP_H_ */
