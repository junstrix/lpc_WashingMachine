#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer32.h"
//#include "timer16.h"
#include "stepmotor.h"
#include "washingMachine.h"
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

char const *UARTSendBuffer;  //发送给上位机的 buffer

/* 返回当前洗衣模式*/
char wm_mode(void)
{
	if (UARTBuffer[0]=='A')
	{
		return 'A';
	}
	else if (UARTBuffer[0]=='B')
	{
		return 'B';
	}
	else if (UARTBuffer[0]=='C')
	{
		return 'C';
	}
	else if (UARTBuffer[0]=='D')
	{
		return 'D';
	}
	else {
		return '0';
	}
}
/*返回当前水量*/
char wm_water_size(void)
{
	if (UARTBuffer[2]=='1')
	{
		return '1';
	}
	else if (UARTBuffer[1]=='2')
	{
		return '2';
	}
	else if (UARTBuffer[1]=='3')
	{
		return '3';
	}
	else {
		return '0';
	}
}
/*洗衣处理函数*/
void wm_op()
{
	switch(wm_mode())
	{
		case 'A': /*标准*/
			wm_op_stard();
			break;	
		case 'B':/*大衣物*/
			wm_op_largeClothing();
			break;
		case 'C':/*毛衣*/
			wm_op_sweater();
			break;
		case 'D':/*快速洗涤*/
			wm_op_fastWash();
			break;
		default:
			break;
	}
}
/*进水*/
/*参数：count->次数 freq->时间延时*/
/*时间 = count * (freq*2) */
void wm_water_in(int count,int freq)
{
	UARTSendBuffer="a20111";
	UARTSend((uint8_t *)UARTSendBuffer,6);
	int i;
	for (i = 0; i < count; ++i)
	{
                delay32Ms(0,freq);
		GPIOSetValue(PORT2,8,0);
		delay32Ms(0,freq);
		GPIOSetValue(PORT2,8,1);
	}
}

/*洗涤*/
void wm_wash(int count)
{
	UARTSendBuffer="b20111";
	UARTSend((uint8_t *)UARTSendBuffer,6);
	for (int i = 0; i < count; ++i)
	{
		StepmotorMovePitch(StepMotor_1,MOTOR_CW);
		delay32Ms(0,5000);
	}
	for (int i = 0; i < count; ++i)
	{
		StepmotorMovePitch(StepMotor_1,MOTOR_CCW);
		delay32Ms(0,5000);
	}
}

/*排水*/
/*参数：count->次数 freq->时间延时*/
/*时间 = count * (freq*2) */
void wm_drain(int count, int freq)
{
	UARTSendBuffer="c20011";
	UARTSend((uint8_t *)UARTSendBuffer,6);
	int i;
	for (i = 0; i < count; ++i)
	{
		GPIOSetValue(PORT2,8,0);
		delay32Ms(0,freq);
		GPIOSetValue(PORT2,8,1);
		delay32Ms(0,freq);
	}
}

/*脱水*/
void wm_water_out(int count)
{
	UARTSendBuffer="d20011";
	UARTSend((uint8_t *)UARTSendBuffer,6);
	for (int i = 0; i < count; ++i)
	{
		StepmotorMovePitch(StepMotor_1,MOTOR_CW);
		delay32Ms(0,1000);
	}
}
/*完成*/
void wm_done(void)
{
	UARTSendBuffer="e20011";
	UARTSend((uint8_t *)UARTSendBuffer,6);
	GPIOSetValue(PORT3,3,0);
	delay32Ms(0,1000);
	GPIOSetValue(PORT3,3,1);
	delay32Ms(0,1000);
	GPIOSetValue(PORT3,3,0);
	delay32Ms(0,1000);
	GPIOSetValue(PORT3,3,1);
	/*NVIC_SystemReset();*/
}

/*标准*/
void wm_op_stard(void)
{
	UARTBuffer[0]='0'; //清空
	wm_water_in(5,1000);//亮20次延时1秒
	wm_wash(2);
	wm_drain(5,1000);
	wm_water_out(20);
	wm_done();
}
/*大衣物*/
void wm_op_largeClothing()
{
	/*UARTSendBuffer="大衣物模式";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //清空*/
	/*wm_water_in(5,1000);//亮20次延时1秒*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
/*毛衣*/
void wm_op_sweater()
{
	/*UARTSendBuffer="毛衣模式";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //清空*/
	/*wm_water_in(5,1000);//亮20次延时1秒*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
/*快速洗涤*/
void wm_op_fastWash()
{
	/*UARTSendBuffer="快速模式";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //清空*/
	/*wm_water_in(5,1000);//亮20次延时1秒*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
