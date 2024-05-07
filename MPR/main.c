/*
 * MPR.c
 *
 * Created: 07.05.2024 13:07:28
 * Author : vladimir.muzik
 */ 

#include <avr/io.h>
#include "makra.h"
#include "dispctrl_exp.h"
#include <avr/delay.h>
#include "spin.h"
int i;
int j;
//#define F_CPU 16000000

int main(void)
{
    display_init();
    DDRA = 0x00;
	PORTA = 0xff;
    while (1) 
    {
    if (i == 1)display_show_number(6542);
	
	if (TESTNEGBIT(SPINACE,s0)){i=1; 
		j=0;
		}
		
	if (TESTNEGBIT(SPINACE,s7))
		{
			i=0;
			j=1;
			
			
				
			}
		if (j==1)
		{
		display_show_number(3210);
		}
		}
		
		if (TESTNEGBIT(SPINACE,s3))
		{
		i=0;
		j=0;
		}
	
	}
	
	


