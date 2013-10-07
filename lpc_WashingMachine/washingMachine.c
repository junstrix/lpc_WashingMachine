#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer32.h"
//#include "timer16.h"
#include "stepmotor.h"
#include "washingMachine.h"
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

char const *UARTSendBuffer;  //���͸���λ���� buffer

/* ���ص�ǰϴ��ģʽ*/
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
/*���ص�ǰˮ��*/
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
/*ϴ�´�����*/
void wm_op()
{
	switch(wm_mode())
	{
		case 'A': /*��׼*/
			wm_op_stard();
			break;	
		case 'B':/*������*/
			wm_op_largeClothing();
			break;
		case 'C':/*ë��*/
			wm_op_sweater();
			break;
		case 'D':/*����ϴ��*/
			wm_op_fastWash();
			break;
		default:
			break;
	}
}
/*��ˮ*/
/*������count->���� freq->ʱ����ʱ*/
/*ʱ�� = count * (freq*2) */
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

/*ϴ��*/
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

/*��ˮ*/
/*������count->���� freq->ʱ����ʱ*/
/*ʱ�� = count * (freq*2) */
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

/*��ˮ*/
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
/*���*/
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

/*��׼*/
void wm_op_stard(void)
{
	UARTBuffer[0]='0'; //���
	wm_water_in(5,1000);//��20����ʱ1��
	wm_wash(2);
	wm_drain(5,1000);
	wm_water_out(20);
	wm_done();
}
/*������*/
void wm_op_largeClothing()
{
	/*UARTSendBuffer="������ģʽ";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //���*/
	/*wm_water_in(5,1000);//��20����ʱ1��*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
/*ë��*/
void wm_op_sweater()
{
	/*UARTSendBuffer="ë��ģʽ";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //���*/
	/*wm_water_in(5,1000);//��20����ʱ1��*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
/*����ϴ��*/
void wm_op_fastWash()
{
	/*UARTSendBuffer="����ģʽ";*/
	/*UARTSend((uint8_t *)UARTSendBuffer,BUFSIZE);*/
	/*UARTBuffer[0]='0'; //���*/
	/*wm_water_in(5,1000);//��20����ʱ1��*/
	/*wm_wash(2);*/
	/*wm_drain(5,1000);*/
	/*wm_water_out(20);*/
	/*wm_done();*/
}
