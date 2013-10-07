#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer32.h"
// #include "timer16.h"
#include "washingMachine.h"
#include "stepmotor.h"

extern volatile uint32_t	UARTCount;
extern volatile uint8_t         UARTBuffer[BUFSIZE];

int main (void) 
{
	SystemInit();
	init_timer32(0,0);
	GPIOInit();
	InitStep();
	GPIOSetDir(PORT2,8,1);
	GPIOSetDir(PORT3,3,1); //·äÃùÆ÷
	UARTInit(9600);
	while (1)
	{
		wm_op();
	}
}