#include "lpc17xx.h"

int interval = 100; // value of the interval in ms
void (*CB_on_Tick)(void);

void TIMER0_IRQHandler(void)
{
	LPC_TIM0->IR = 1;
	CB_on_Tick();
}

void TIMER0_init(void (*RegisterCB)(void))
{
	// the peripheral clock for the timer is set to the core clock frequency (3MHz)
	CB_on_Tick = RegisterCB;
	
	LPC_SC->PCONP |= 0x1;	
	LPC_SC->PCLKSEL0 |= 0x4;
	LPC_SC->PCLKSEL0 &= 0xfffffff7;
	
	LPC_TIM0->MCR = 0x3;
	
	LPC_TIM0->MR0 = interval - 1;
	LPC_TIM0->PR = 3000 - 1; // counts 1ms
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	LPC_TIM0->TCR = 1;
	
}

int get_interval(void)
{
	return interval;
}

void set_interval(uint32_t value)
{
	interval = value;
}
