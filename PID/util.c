#include "util.h"

int StringToInt(char * string_conv, int length)
{
	int radix = 1;
	int conv_num = 0;
	
	if(string_conv == 0)
		return -1;
	
	while(length--)
	{
		if(string_conv[length] >= '0' && string_conv[length] <= '9')
		{
			conv_num += (string_conv[length] - '0') * radix;
			radix *= 10;
		}
		else
			return -1;
	}
	
	return conv_num;
}

int IntToString(int num_conv, char* conv_string)
{
	int d[10] = {0};
	int i = 0, n = 0;
	
	if(conv_string == 0)
		return -1;
	
	if(num_conv < 0)
		return -1;
	
	while(num_conv != 0)
	{
		d[i] = num_conv % 10;
		num_conv /= 10;
		i++;
	}
	
	n = i;
	
	while(--i >= 0)
	{
		*conv_string = d[i] + '0';
		conv_string++;
	}
	
	*conv_string = 0;
	conv_string -= n;
	
	return 1;
	
}
int StrLength(char * string_meas)
{
	int n = 0;
	
	if(string_meas == 0)
		return -1;
	
	while(string_meas[n] != 0)
		n++;
	
	return n;
}
	
void ReadTillChar(char c, char* read_msg)
{
	int n = 0;
	UART0_RecvEnable();
	n = UART0_WaitForChar('\r');
	UART0_RecvDisable();		
	UART0_Read(read_msg, n);
}
