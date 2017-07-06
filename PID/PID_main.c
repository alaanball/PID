#include "lpc17xx.h"
#include "util.h"
#include <stdio.h>

float kp = 50, ki = 1500, kd = 2, dt = 0.25; // 200, 120, 1 works; old: 1, 750, 0.1
float integral_term = 0, prop_term = 0, derv_term = 0, PID_max = 65535;
float rpm_current = 0, rpm_error = 0, prev_rpm_error = 0, ref_rpm = 55, rpm_max = 120;// measured rpm_max for 5V; currently, anticlockwise is negative calc_rpm = 0, separation;
int PID_op = 0, duty_set = -50, per_set = 200, duty_old = 0;
int encoder_ppr = 10, direction = 1, dir1 = 1;
int timer_count = 0, adder = 10, b_adder = 10;
float average_rpm = 0, sum = 0, prev_average = 0; int avg_count = 8; int avg_c = 0;
int x=0;
void (*callBack)(void);

char msg[50] = "";
int code = 0;

int calc_PID(void)
{ 
	integral_term += dt*ki*rpm_error;
	prop_term = kp*rpm_error;
	derv_term = kd*(rpm_error - prev_rpm_error)/dt;
	prev_rpm_error = rpm_error;
	
	PID_op = (int) (integral_term + prop_term + derv_term);
	if(PID_op > PID_max)
		return PID_max;
	if(PID_op < -PID_max)
		return -PID_max;
	else
		return PID_op;
}

void dirn_change_handler(void)
{
	direction *= 1;
}
void PID_sequence(void)
{
	//rpm_current = ((float)(GPIO2_getCount())/ (encoder_cpr)) * 60 / dt;
	//GPIO2_setCount(0);
	rpm_current = (float) QEI_get_RPM();

	//dir1 = QEI_get_dirn();
	//dir1 = 1;
	rpm_error = ref_rpm -   rpm_current;
	
	if(!(rpm_error < 3 && rpm_error > -3))
	{
	duty_set = (int) (calc_PID());
	
	if(duty_set != duty_old)
	{
		MCPWMConfig(per_set, (int) (duty_set * 50) / PID_max);
		MCPWMStart();
	}
	
	duty_old = duty_set;
	}
	avg_c++;
	//x++;
	avg_c = (avg_c) % avg_count;
	//sum +=rpm_current;
	//prev_average=sum/x;
	if(avg_c <= avg_count - 1)
	{
		average_rpm += rpm_current;
		
	}
	if(avg_c==avg_count - 1)
	{
		sprintf(msg, " %f \r ", average_rpm/avg_count);
		//prev_average = average_rpm/avg_count;
		UART0_Send(msg) ;
		average_rpm=0;
	}
}

void openLoop_sequence(void)
{
//	rpm_current = ((float)(GPIO2_getCount())/ (encoder_cpr)) * 60 / dt;
//	GPIO2_setCount(0);
	rpm_current = (float) QEI_get_RPM();
	
	//duty_set = (int)((ref_rpm * 50)/rpm_max);
	if(duty_set != duty_old)
	{
		MCPWMConfig(per_set, duty_set);
		MCPWMStart();
	}
	
	duty_old = duty_set;
	
	timer_count = (timer_count + 1) % 10;
	
	if(timer_count == 0)
		duty_set+=adder;
	
	if(duty_set == 50)
		adder = -b_adder;
	
	if(duty_set == -50)
		adder = b_adder;	
	
	dir1 = QEI_get_dirn();
	sprintf(msg, "%d, %f \r", duty_set, rpm_current*dir1 );
	UART0_Send(msg);
}

int main(void)
{	
	SystemInit();
	SystemCoreClockUpdate();
	
	UART0_Init();
	UART0_Configure(57600, SERIAL_8N1);
	
//	do
//	{
//		while(UART0_Send("\n Type 0 (open loop) or 1 (PID) and press enter \n") == 0);
//		while(UART0_TransmitStatus() != 0);
//		
//		ReadTillChar('\r', msg);
//		
//		code = StringToInt(msg, StrLength(msg) - 1);
//	}
//	while(code != 0 && code != 1);
	
	code = 1;
	
	if(code == 0)
	{
		//while(UART0_Send("Using open loop... \n") == 0);
		callBack = openLoop_sequence;
	}
	else
	{
		//while(UART0_Send("Using PID... \n") == 0);
		callBack = PID_sequence;		
	}
	while(UART0_TransmitStatus() != 0);
	
	//while(UART0_Send("Press S to Stop \n \n") == 0);
	while(UART0_TransmitStatus() != 0);
	
	MCPWMInit();
	// GPIO2_init();	
	//TIMER0_init(callBack);
	QEI_set_PPR(encoder_ppr);
	QEI_Init(callBack, dirn_change_handler);
	
	UART0_RecvEnable();
	UART0_WaitForChar('s');
	
	//ref_rpm = 0;
	while(1);
}


