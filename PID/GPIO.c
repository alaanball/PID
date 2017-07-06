#include "lpc17xx.h"
#include <stdio.h>
#include "UART0.h"

int count = 0;
///char msg[50] = "hello";

void GPIO2_init(void)
{
	LPC_PINCON->PINSEL4 &= 0xfffffffc; // port 2.0
	//LPC_PINCON->PINMODE4 &= 0xfffffffc; // pull up resistor - default high
	
	LPC_GPIO2->FIODIR = 0; // all port pins are input
	
	LPC_GPIOINT->IO2IntEnR = 1;  // enable interrupt on rising edge on p2.0
	LPC_GPIOINT->IO2IntEnF = 1;	 // enable interrupt on falling edge on p2.0
	
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void) // EINT3 handler is also used for G4PIO interrupt handling
{
	count++;
	
	//sprintf(msg, "%d\r", count);
	//while(UART0_Send(msg) < 0);
	//while(UART0_TransmitStatus() != 0);
//	while(UART0_TransmitStatus() != 0);
	
	LPC_GPIOINT->IO2IntClr = 1;
}

void GPIO2_setCount(int set_val)
{
	count = set_val;
}

int GPIO2_getCount(void)
{
	return count;
}
