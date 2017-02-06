/**
  ******************************************************************************
  * @file    USART/Printf/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include "led.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**-------------------------------------------------------
  * @������ EXTI0_IRQHandler
  * @����   �����ⲿ�ж���0���жϣ�����4�жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void EXTI0_IRQHandler(void)  /* Key 4 */
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* �ӳ�ȥ�� */
        delay(150000);
        if(KEY4IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_9);//��һ����
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);	 //��3����
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);	 //��4����
        }
        /* �ȴ������ͷ� */
        while(KEY4IBB == 0);
        /* �ӳ�ȥ�� */
        delay(150000);        

        /* Clear the EXTI Line 0 */
        /* ����жϹ����־λ������ᱻ��Ϊ�ж�û�б������ѭ���ٴν����ж� */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**-------------------------------------------------------
  * @������ EXTI15_10_IRQHandler
  * @����   �����ⲿ�ж���10-15���жϣ�����3�жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void EXTI15_10_IRQHandler(void) /* Key 3 */
{
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        /* �ӳ�ȥ�� */
        delay(150000);
            if(KEY3IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);//��һ����
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
        }
        /* �ȴ������ͷ� */
        while(KEY3IBB == 0);
        /* �ӳ�ȥ�� */
        delay(150000);

        /* Clear the EXTI Line 13 */  
        /* ����жϹ����־λ������ᱻ��Ϊ�ж�û�б������ѭ���ٴν����ж� */
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}

/**-------------------------------------------------------
  * @������ EXTI9_5_IRQHandler
  * @����   �����ⲿ�ж���8���жϣ�����1�жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void EXTI9_5_IRQHandler(void) /* Key 1 */
{
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        /* �ӳ�ȥ�� */
        delay(150000);
        if(KEY1IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);//��һ����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
				if(StartFlag==GAMEOVER)	StartFlag=GAMEWAITING;
        }
        /* �ȴ������ͷ� */
				seed=0;
        while(KEY1IBB == 0) {seed++;if(seed>5000) seed=0;}
				
        /* �ӳ�ȥ�� */
        delay(150000);        

        /* Clear the EXTI Line 4 */
        /* ����жϹ����־λ������ᱻ��Ϊ�ж�û�б������ѭ���ٴν����ж� */
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}

/**-------------------------------------------------------
  * @������ EXTI3_IRQHandler
  * @����   �����ⲿ�ж���3���жϣ�����2�жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void EXTI3_IRQHandler(void) /* Key 2 */
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        /* �ӳ�ȥ�� */
        delay(150000);
        if(KEY2IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);//��һ����
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //�ڶ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //��3����
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //��4����
						if(StartFlag==GAMEING){   //�������Ϸ��
								StartFlag=GAMEPAUSE;
							  //TIM_Cmd(TIM5, DISABLE); 
						}
						else if(StartFlag==GAMEPAUSE){   //�������Ϸ��
								StartFlag=GAMEING;
								//TIM_Cmd(TIM5, ENABLE); 
				}						
        }
        /* �ȴ������ͷ� */
        while(KEY2IBB == 0);
        /* �ӳ�ȥ�� */
        delay(150000);        

        /* Clear the EXTI Line 4 */
        /* ����жϹ����־λ������ᱻ��Ϊ�ж�û�б������ѭ���ٴν����ж� */
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

