/****************************************Copyright (c)*****************************************************
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
** ��   ��   ��: stepmotor.c
** ��        ��: ��������������.
**
**------------------------------------------------------------------------------------------------------
** ������: �¿���
** ��  ��: 0.0.1
** �ա���: 2011.04.31
********************************************************************************************************/

#include "LPC11xx.h"
#include "stepmotor.h"
#include "gpio.h"
#include "timer32.h"

/*****************************************************************************
**     �������
******************************************************************************/
uint32_t step1_ccw[8] = {0x02,0x06,0x04,0x0c,0x08,0x48,0x40,0x42};
uint32_t step1_cw[8] = {0x42,0x40,0x48,0x08,0x0c,0x04,0x06,0x02};
uint32_t step2_ccw[8] = {0x80,0x180,0x100,0x300,0x200,0xa00,0x800,0x880};
uint32_t step2_cw[8] = {0x880,0x800,0xa00,0x200,0x300,0x100,0x180,0x80};
uint32_t step3_ccw[8] = {0x10,0x30,0x20,0x60,0x40,0xc0,0x80,0x90};
uint32_t step3_cw[8] = {0x90,0x80,0xc0,0x40,0x60,0x20,0x30,0x10};
/******************************************************************************
**   ��ʼ������������
******************************************************************************/
void InitStep()
{
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  LPC_IOCON->JTAG_TDI_PIO0_11 &= ~(0x07);           //����P0.11ΪGPIO
  LPC_IOCON->JTAG_TDI_PIO0_11 |= 0x01;

  LPC_GPIO0->DIR &= ~(0xbce);                       //����GPIOΪ���
  LPC_GPIO2->DIR &= ~(0xf0);
  LPC_GPIO0->DIR |= 0xbce;
  LPC_GPIO2->DIR |= 0xf0;

  //init_timer16( 1, 0 );                             //��ʼ��16λ��ʱ��1
}
/*****************************************************************************
**  ������ת��һ���ݾ�
**  stepNumΪ���������
**  stepDirΪ������ת������
*****************************************************************************/
void StepmotorMovePitch(uint8_t stepNum, uint8_t StepDir)     //������ת��һ���ݾ�
{
  uint8_t i, j;
  
  for( j=0; j<8; j++ )
  {
    if( StepDir == MOTOR_CCW )                              //��ʱ��ת��
    {
      if( stepNum == StepMotor_1 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR1_PIN;
          LPC_GPIO0->DATA |= step1_ccw[i];
          delay32Ms(0,1);
        }
      }
      else if( stepNum == StepMotor_2 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR2_PIN;
          LPC_GPIO0->DATA |= step2_ccw[i];
          delay32Ms(0,1);
        }    
      }
      else if( stepNum == StepMotor_3 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO2->DATA &= STEPMOTOR3_PIN;
          LPC_GPIO2->DATA |= step3_ccw[i];
          delay32Ms(0,1);
        }    
      }
    }
    else if( StepDir == MOTOR_CW )                           //˳ʱ��ת��
    {
      if( stepNum == StepMotor_1 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR1_PIN;
          LPC_GPIO0->DATA |= step1_cw[i];
        delay32Ms(0,1);
        }
      }
      else if( stepNum == StepMotor_2 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO0->DATA &= STEPMOTOR2_PIN;
          LPC_GPIO0->DATA |= step2_cw[i];
            delay32Ms(0,1);
        }    
      }
      else if( stepNum == StepMotor_3 )
      {
        for( i=0; i<8; i++ )
        {
          LPC_GPIO2->DATA &= STEPMOTOR3_PIN;
          LPC_GPIO2->DATA |= step3_cw[i];
           delay32Ms(0,1);
        }    
      }
    }
  }    
  return;
}