/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM֮STM32�����Ŷ�  JESSE 
* Version            :
* Date               :
* Description        : STM32����III�ſ�����    ����
*******************************************************************************/

#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "ARMJISHU_TouchScreen_ADS7843.h"
#include "SZ_STM32F103ZE_LIB.h"
#include "I2C_MPU6050.h"
#include "led.h"

//#include "game_control.h"
extern uint8_t MoveDirection;	//�ƶ�����
extern u8 StartFlag;
u8 timeUpFlag=0;//��1s�ƶ�һ�£�1s��ʱ���ı�־λ

#define dirNone 0
#define dirLeft 1
#define dirRight 2
#define dirFront 3
#define dirBack 4
#define GAMEOVER			1
#define GAMEWAITING   2
#define GAMEING				3
#define GAMEPAUSE			4
#define NONE					0
#define LEFT					1
#define RIGHT					2
#define UP						3
#define DOWN					4
/*-------------------------------------------------------------------------------*/
#define BmpHeadSize (54)
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

GPIO_InitTypeDef GPIO_InitStructure;

typedef struct _HEADCOLOR
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR; 

/**-------------------------------------------------------
  * @������ LCD_DrawBmp565Pic
  * @����   ��ָ�����귶Χ��ʾһ��Image2Lcdת����RGB565ͼƬ
  * @����   StartX     ����ʼ����
  *         StartY     ����ʼ����
  *         Width      ͼƬ���
  *         Height     ͼƬ�߶�
  *         BmpAddress ͼƬ��������ͷָ��(�����ļ���Ϣ)
  * @����ֵ ��
  * ����    www.armjishu.com
***------------------------------------------------------*/
void LCD_DrawBmp565Pic(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, const uint16_t *BmpAddress)
{
//    uint32_t total;
    uint32_t i, j;
    uint32_t pointor;
    uint16_t line;

    //LCD_DEBUG_PRINTF("LCD_DrawBmp565Picture StartX %d StartY %d Width %d Height %d \n\r"
    //                              , StartX, StartY, Width, Height);

    line=StartX;
#if 0  // ���ַ��������ٶȿ죬���ǼĴ�����ΪLCD����оƬ�Ĳ�ͬ����ͬ
    total = Width * Height;

    LCD_WriteReg(0x0044,0xEF00 + StartX);        //Specify the start/end positions of the window address in the horizontal direction by an address unit
    LCD_WriteReg(0x0045,StartY);        //Specify the start positions of the window address in the vertical direction by an address unit 
    LCD_WriteReg(0x0046,StartY + Width -1 );        //Specify the end positions of the window address in the vertical direction by an address unit 

    ili9320_SetCursor(line, StartY);
  	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

    for (i=0;i<total;i++)
    {
        LCD_WriteRAM(BmpAddress[i]);
    }

    LCD_WriteReg(0x0044,0xEF00);        //Specify the start/end positions of the window address in the horizontal direction by an address unit
    LCD_WriteReg(0x0045,0x0000);        //Specify the start positions of the window address in the vertical direction by an address unit 
    LCD_WriteReg(0x0046,0x013F);        //Specify the end positions of the window address in the vertical direction by an address unit 
#else
    pointor = 0;
    for (i=0;i<Height;i++)
    {
        ili9320_SetCursor(line, StartY);
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        for (j=0;j<Width;j++)
        {
            LCD_WriteRAM(BmpAddress[pointor]);
            pointor++;
        }
        line++;
    }
#endif
}

/**-------------------------------------------------------
  * @������ LCD_Image2LcdDrawBmp565Pic
  * @����   ��ָ�����귶Χ��ʾһ��Image2Lcdת����Bmp565ͼƬ
  * @����   StartX     ����ʼ����
  *         StartY     ����ʼ����
  *         BmpAddress ͼƬ�ļ�ͷ�ĵ�ַָ��(�����ļ���Ϣ)
  * @����ֵ ��
  * ����    www.armjishu.com
***------------------------------------------------------*/
void LCD_Image2LcdDrawBmp565Pic(uint16_t StartX, uint16_t StartY, const uint8_t *BmpAddress)
{
    HEADCOLOR * BmpHeadr;

    uint16_t Width, Height;
    uint8_t *BmpData;

    BmpHeadr = (HEADCOLOR *)BmpAddress;
    Width  = BmpHeadr->w;
    Height = BmpHeadr->h;
    BmpData = (uint8_t *)(BmpAddress + sizeof(HEADCOLOR));
    if((BmpHeadr->is565 == 1) && (BmpHeadr->gray == 16))
    {
        LCD_DrawBmp565Pic(StartX, StartY, Width, Height, (const uint16_t *)BmpData);
    }
    else
    {
//        LCD_DEBUG_PRINTF("\n\r LCD_Image2LcdDrawBmp565Pic: Not Image2Lcd or Bmp565 format.");
    }
}


void STM32_Shenzhou_COMInit(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(USART1, USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}



void ili9320_DrawPicture1(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic) 
{  
	u32 i, total; 
	u16 x,y;  
	// ili9320_SetWindows(StartX,StartY,EndX,EndY); 
	// ili9320_SetCursor(StartX,StartY); 
	x=StartX; 
	y=StartY; 
	// LCD_WriteRAM_Prepare(); 
	total = EndX*(EndY-StartY+1); 
	for (i=0;i<total;i++) 
	{  
		ili9320_SetPoint(x,y,*pic++); 
		y++; 
		if(y > EndY) 	
		{  
			x++;  
			y=StartY; 
		}  
	// LCD_WriteRAM(*pic++); 
	} 
} 

void Printf_Init(void)
{
  USART_InitTypeDef USART_InitStructure;          
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM32_Shenzhou_COMInit(&USART_InitStructure);

  //printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  //printf(" WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d",__STM32F10X_STDPERIPH_VERSION_MAIN,
		//	__STM32F10X_STDPERIPH_VERSION_SUB1,	__STM32F10X_STDPERIPH_VERSION_SUB2);
  //printf("\n\r ��Ʒ�ڲ�Flash��СΪ��%dK�ֽڣ� \t www.armjishu.com",*(__IO uint16_t*)(0x1FFFF7E0));
  SystemCoreClockUpdate();
  //printf("\n\r ϵͳ�ں�ʱ��Ƶ��(SystemCoreClock)Ϊ��%dHz.\n\r",SystemCoreClock);
}

int main(void)
{   
	uint8_t directionTemp=0;
/*-------------------------Һ������ʼ��-------------------------------*/
	Printf_Init();
  ili9320_Initializtion();
  ili9320_Clear(0x9EDD);
/*-------------------------��ʼ��ӭ����-------------------------------*/
	gamePicInit();
	Delay(0x3ff);		
/*--------------------------���ٶȼ�ģ���ʼ��-------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_MPU6050_Init();
	InitMPU6050();
	LED_config();
	NVIC_GroupConfig();
		
    /* ��ʼ�����ذ���Ϊ�ж�ģʽ */
    SZ_STM32_KEYInit(KEY1, BUTTON_MODE_EXTI);
    SZ_STM32_KEYInit(KEY2, BUTTON_MODE_EXTI);	
    SZ_STM32_KEYInit(KEY3, BUTTON_MODE_EXTI);
    SZ_STM32_KEYInit(KEY4, BUTTON_MODE_EXTI);
/*---------------------------TIM5��ʱ����ʼ��-----------------------------*/
  TIM5_Init();
	
	
//Init_Snake();
/*---------------------------��whileѭ����ʼ-------------------------------*/
  while (1){
		if(StartFlag==GAMEWAITING) 	Init_Snake();
		if(timeUpFlag){
			timeUpFlag=0;		
/*--------------------------1s��ʱ��------------------------------*/
			directionTemp=directionJudge();
			if(directionTemp){
				if(MoveDirection==UP && directionTemp!=DOWN) MoveDirection=directionTemp;
				else if(MoveDirection==DOWN && directionTemp!=UP) MoveDirection=directionTemp;
				else if(MoveDirection==LEFT && directionTemp!=RIGHT) MoveDirection=directionTemp;	
				else if(MoveDirection==RIGHT && directionTemp!=LEFT) MoveDirection=directionTemp;				
			}
			gameMain();
		}
  }
}
