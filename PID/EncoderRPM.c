#include "lpc17xx.h"
#include "UART0.h"
#include "GPIO.h"
#include <stdio.h>

int main(void)
{	
	SystemInit();
	SystemCoreClockUpdate();
	
	UART0_Init();
	UART0_Configure(57600, SERIAL_8N1);
	
	GPIO2_init();	

	while(1);
}
