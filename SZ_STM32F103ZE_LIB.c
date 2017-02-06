/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��SZ_STM32F103ZE_LIB.c
 * ����    ���ṩSTM32F103ZE����III�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ����    ��www.armjishu.com 
**********************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "SZ_STM32F103ZE_LIB.h"
#include "I2C_MPU6050.h"
#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "ARMJISHU_TouchScreen_ADS7843.h"
#include "led.h"
// #include "picture.h"
// #include "food.h"
// #include "SNAKE.h"
// #include "BODY.h"
USART_TypeDef* COM_USART[COMn] = {SZ_STM32_COM1, SZ_STM32_COM2}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {SZ_STM32_COM1_TX_GPIO_PORT, SZ_STM32_COM2_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {SZ_STM32_COM1_RX_GPIO_PORT, SZ_STM32_COM2_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {SZ_STM32_COM1_CLK, SZ_STM32_COM2_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {SZ_STM32_COM1_TX_GPIO_CLK, SZ_STM32_COM2_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {SZ_STM32_COM1_RX_GPIO_CLK, SZ_STM32_COM2_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {SZ_STM32_COM1_TX_PIN, SZ_STM32_COM2_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {SZ_STM32_COM1_RX_PIN, SZ_STM32_COM2_RX_PIN};
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN_NUM[LEDn] = {LED1_PIN_NUM, LED2_PIN_NUM, LED3_PIN_NUM, LED4_PIN_NUM};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT, KEY3_BUTTON_GPIO_PORT, KEY4_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN_NUM[BUTTONn] = {KEY1_BUTTON_PIN_NUM, KEY2_BUTTON_PIN_NUM, KEY3_BUTTON_PIN_NUM, KEY4_BUTTON_PIN_NUM}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN, KEY3_BUTTON_PIN, KEY4_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK, KEY3_BUTTON_GPIO_CLK, KEY4_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY1_BUTTON_EXTI_LINE, KEY2_BUTTON_EXTI_LINE, KEY3_BUTTON_EXTI_LINE, KEY4_BUTTON_EXTI_LINE};
const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PORT_SOURCE, KEY4_BUTTON_EXTI_PORT_SOURCE};
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PIN_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE, KEY4_BUTTON_EXTI_PIN_SOURCE}; 
const uint16_t BUTTON_IRQn[BUTTONn] = {KEY1_BUTTON_EXTI_IRQn, KEY2_BUTTON_EXTI_IRQn, KEY3_BUTTON_EXTI_IRQn, KEY4_BUTTON_EXTI_IRQn};

/**-------------------------------------------------------
  * @������ delay
  * @����   �򵥵�delay��ʱ����.
  * @����   �ӳ������� 0--0xFFFFFFFF
  * @����ֵ ��
***------------------------------------------------------*/
void delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   ��STM32��USART��ʼ���ײ㺯��
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  ָ��һ����Ա�Ѹ�ֵUSART_InitTypeDef�ṹ���ָ��
  * @����ֵ ��
***------------------------------------------------------*/
void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    /* ʹ��STM32��USART��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

    if (COM == COM1)
    {
        /* ʹ��STM32��USART1��Clockʱ�� */
        RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
    }
    else
    {
        /* ʹ��STM32��USART��Clockʱ�� */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    /* ��ʼ��STM32��USART��TX�ܽţ�����Ϊ���ù���������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    /* ��ʼ��STM32��USART��RX�ܽţ�����Ϊ���ù������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    /* ���ݴ���Ĳ�����ʼ��STM32��USART���� */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    /* ʹ��STM32��USART����ģ�� */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   �����û���STM32��USART��ʼ������
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  BaudRate ���ڵĲ����ʣ�����"115200"
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_COMInit(COM_TypeDef COM, uint32_t BaudRate)
{
  
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = BaudRate;              //���ڵĲ����ʣ�����115200 ��ߴ�4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�����ֳ���(8λ��9λ)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //�����õ�ֹͣλ-֧��1��2��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         //����żУ��  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //˫��ģʽ��ʹ�ܷ��ͺͽ���
  
    __SZ_STM32_COMInit(COM, &USART_InitStructure);  // ����STM32��USART��ʼ���ײ㺯��

    printf("\n\r ############ WWW.ARMJISHU.COM! ############ \n");
 
}
/*
    �������´���,֧��printf����,����Ҫѡ��use MicroLIB	  
*/
#ifndef MicroLIB
#pragma import(__use_no_semihosting)   //û��ʵ��fgetcʱ��Ҫ�����ò���          
/* ��׼����Ҫ��֧�ֺ��� ʹ��printf()���Դ�ӡ����Ҫʵ�ָú��� */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* �ض���fputc���� ���ʹ��MicroLIBֻ��Ҫ�ض���fputc�������� */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(SZ_STM32_COM1, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(SZ_STM32_COM1, (uint8_t) ch);

    return ch;
}
/*
����ֱ��ʹ��putchar
����Ҫ�ٶ��� int putchar(int ch)����Ϊstdio.h�������¶���
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 

/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5�ж�������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void NVIC_TIM5Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}


/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5����������ÿ��������ж�һ�� 
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* ---------------------------------------------------------------
    TIM4 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 7200, TIM2 counter clock = 7.2 MHz
    --------------------------------------------------------------- */

    /* Time base configuration */
    //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
    TIM_TimeBaseStructure.TIM_Period = (7000 - 1);
    // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
    // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
    // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //��ʼ����ʱ��5
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[���TIM5����жϱ�־] */
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    /* TIM IT enable */ //������ж�
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM5, ENABLE);  //������ʹ�ܣ���ʼ����

    /* �жϲ������� */
    NVIC_TIM5Configuration();
}


/**-------------------------------------------------------
  * @������ TIM5_IRQHandler
  * @����   TIM5�жϴ�������ÿ���ж�һ�� 
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM5_IRQHandler(void)
{
    /* www.armjishu.com ARM������̳ */
    static u32 counter = 0;

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
				
				timeUpFlag=1;
    }
}
/**-------------------------------------------------------
  * @������ NVIC_GroupConfig
  * @����   ����NVIC�ж����ȼ����麯��.
  *         Ĭ������Ϊ1���ر�ʾ�����ȼ�, 3���ر�ʾ�����ȼ�
  *         �û����Ը�����Ҫ�޸�
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
  /* ����NVIC�ж����ȼ�����:
     - 1���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0 �� 1 
     - 3���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0..7
     - ��ֵԽ�����ȼ�Խ�ߣ�ȡֵ�����Ϸ���Χʱȡ��bitλ */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
/*==================================================================================
    NVIC_PriorityGroup   |  �����ȼ���Χ  |  �����ȼ���Χ  |   ����
  ==================================================================================
   NVIC_PriorityGroup_0  |      0         |      0-15      |   0 ���ر�ʾ�����ȼ�
                         |                |                |   4 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 ���ر�ʾ�����ȼ�
                         |                |                |   3 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 ���ر�ʾ�����ȼ�
                         |                |                |   2 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 ���ر�ʾ�����ȼ�
                         |                |                |   1 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |      0-15      |      0         |   4 ���ر�ʾ�����ȼ�
                         |                |                |   0 ���ر�ʾ�����ȼ�   
  ==================================================================================*/
}
/**-------------------------------------------------------
  * @������ SZ_STM32_KEYInit
  * @����   ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������
  *         ��������ʱΪ�͵�ƽ0
  * @����1  KEY1  ��Ӧ�������ϵ�һ��KEY����
  *         KEY2  ��Ӧ�������ϵڶ���KEY����
  *         KEY3  ��Ӧ�������ϵ�����KEY����
  *         KEY4  ��Ӧ�������ϵ��ĸ�KEY����
  * @����2  BUTTON_MODE_GPIO  ����Ϊ��ͨ����ܽ�ʹ��
  *         BUTTON_MODE_EXTI  ����Ϊ�ⲿEXTI�ж�ʹ��
  *                           ��Ҫ�����жϴ�����
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_KEYInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    /* ʹ��KEY������ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure Button pin as input floating */
    /* ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    /* ��ʼ��KEY����Ϊ�ж�ģʽ */
    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        /* ��KEY������Ӧ�Ĺܽ����ӵ��ڲ��ж��� */    
        GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        /* ��KEY��������Ϊ�ж�ģʽ���½��ش����ж� */    
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        /* ��KEY�������ж����ȼ�����Ϊ��� */  
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 
    }
}
#ifdef  USE_FULL_ASSERT
// ��Ҫ�ڹ�������Option(��ݼ�ALT+F7)C++����ҳ��define������"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

