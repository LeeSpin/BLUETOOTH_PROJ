/*
 * ap.c
 *
 *  Created on: 2018. 10. 18.
 *      Author: leejinhan
 */

//최종 목표는 serial, dcmi

#include "ap.h"

void apInit(void)
{
	//printf("test....\n");
	uartOpen(_DEF_UART1, 115200);
	//ledInit();
}

void apMain()
{
	int len=0;
	while(1)
	{

	  if(uartAvailable(_DEF_UART1) > 0)
	  {
		  len = uartPrintf(_DEF_UART1, "rx = 0x%X\n", uartRead(_DEF_UART1));

	  }
	  ledToggle(0);
	  delay(len * 1000);
	}
}
