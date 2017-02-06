/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM֮STM32�����Ŷ�  JESSE 
* Version            :
* Date               : 
* Description        : STM32����III�ſ�����    ����
*******************************************************************************/
#include "led.h"

/*��ʼ��4��LED��*/
void LED_config(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;

	  /* ��������III��LED��ʹ�õ�GPIO�ܽ�ģʽ*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE); /*ʹ��LED��ʹ�õ�GPIOʱ��*/

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  /*����III��ʹ�õ�LED����ص�GPIO�ڳ�ʼ��*/
		GPIO_SetBits(GPIOF,GPIO_Pin_6);//��һ����
		GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //�ڶ�����
		GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
}

void LED_change(uint8_t MoveDirection){
	switch(MoveDirection){
 			case 0:
				GPIO_SetBits(GPIOF,GPIO_Pin_6);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				break;
			case 1:
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				break;	
			case 2:
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				break;		
			case 3:
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				break;				
			case 4:
				GPIO_ResetBits(GPIOF,GPIO_Pin_9);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��4����
				break;	
			default:
				GPIO_SetBits(GPIOF,GPIO_Pin_6);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				break;
	}
}


