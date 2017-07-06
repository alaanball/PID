#include "lpc17xx.h"
#include "util.h"

void (*cb_timer) (void);
void (*cb_dirn) (void);

int tim_millisec = 250, Edges = 4, PPR = 0, RPM = 0; 
	
void QEI_Init(void (*CB_on_timer_ellapse) (void), void (*CB_on_dirn_change) (void))
{
	cb_timer = CB_on_timer_ellapse;
	cb_dirn = CB_on_dirn_change;
	
	LPC_SC->PCONP |= (0x1 << 18);	 // 3 MHz as PCLK
	LPC_SC->PCLKSEL1 |= 0x1;
	LPC_SC->PCLKSEL1 &= ~(0x2);
	
	LPC_PINCON->PINSEL3 |= (1<<8) + (1<<14) + (1<<16);       	// Using MCI0, 1, 2 with pull up resistors
	LPC_PINCON->PINSEL3 &= ~((1<<9) + (1<<15) + (1<<17));			
	
	LPC_QEI->QEILOAD = 3000 * tim_millisec;	// timer reloads every tim_millisec ms
	LPC_QEI->FILTER = 3*4000; 									// pulse must remain for at least 5 us to be recognized
	
	Edges = 4;  // count both transitions
	LPC_QEI->QEIIES = 4;
	
	LPC_QEI->QEIIES = 0xA; 									// enable velocity timer and direction interrupts
	
	NVIC_EnableIRQ(QEI_IRQn);
}

void QEI_IRQHandler(void)
{
	uint32_t Status;
	Status = LPC_QEI->QEIINTSTAT;
	
	if(Status & 2)
	{
		RPM = (1000 * LPC_QEI->QEICAP * 60) / (tim_millisec * PPR * Edges);
		LPC_QEI->QEICLR = 2;
		
		LPC_QEI->QEICON = 4;
		cb_timer();
	}
	else if(Status & 8)
	{
		LPC_QEI->QEICLR = 8;
		//LPC_QEI->QEICON = 4;
		//UART0_Send("change\r");
		cb_dirn();
	}	
}

int QEI_get_RPM(void)
{
	return RPM;
}

void QEI_set_PPR(int ppr_val)
{
	PPR = ppr_val;
}

int QEI_get_dirn(void)
{
	int dir, dirinv, direction;
	
	dir = (LPC_QEI->QEISTAT & 1);
	dirinv = LPC_QEI->QEISTAT & 1;
	
	direction = dir ^ dirinv;
	
	if(direction == 0)
		return -1;
	else
		return 1;
}

