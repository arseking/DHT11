#include <reg52.h>
#include <stdio.h>
#include "Delay.h"
#include "DHT11.h"
#include "UART_Printf.h"  

void main()
{	
	Timer0_Init();	//初始化定时器0
	UART_Init();
	
	Delay_ms(2000);

	
	while(1)
	{
		DHT11_receive();
		
		printf("%s", rec_dat);
		
		Delay_ms(2000);
	}
}