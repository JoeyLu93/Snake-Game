#include "game_control.h"

/*---------------------------------------------------------------*/
void gamePicInit(void){
	LCD_Image2LcdDrawBmp565Pic(0,0,gImage_dd);
	Delay(0x3ff);
}
/*---------------------------------------------------------------*/
void Init_Snake(void)
{
	int i=0,j=0;
	StartFlag=GAMEING;
	SnakeLenth=3;	//����3
	preMoveDirection=DOWN;
	MoveDirection=DOWN;
	SnakeCoordinate[0]=2<<8|0;	//ͷ������(0,0)
	SnakeCoordinate[1]=1<<8|0;
	SnakeCoordinate[2]=0<<8|0;	//β������(11,15)
	for(i=0;SnakeCoordinate[i]!=0;i++){
		DisplayBuffer[16*(SnakeCoordinate[i]>>8&0x00ff)+(SnakeCoordinate[i]&0x00ff)]=1;
	}
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	ili9320_Clear(0x9EDD);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	
	showSnakeHead(SnakeCoordinate[0]);
	showSnakeBody(SnakeCoordinate[1]);
	showSnakeEnd(SnakeCoordinate[2]);
	
	createFood();
	DisplayBuffer[16*FoodCoordinate_x+FoodCoordinate_y]=2;	
	showFood(FoodCoordinate_x,FoodCoordinate_y);	//��ʾʳ��	

}
int myRand(void)
{
	seed=(seed*157+49)%283;
	while(seed>65535) seed-=60000;
	
}
void createFood(void)
{
	do	//����һ����ʼʳ��
	{
		FoodCoordinate_x=myRand()%12;
		FoodCoordinate_y=myRand()%16;		
	}while(DisplayBuffer[16*FoodCoordinate_x+FoodCoordinate_y] != 0);	//��ͣ�ز�������,ֱ�����겻��������
}
void showSnakeHead(uint16_t head)
{		
		int row=((head>>8)& 0x00ff )*20;
		int column=( head & 0x00ff )*20;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		//ili9320_Clear(0x9EDD);
		switch(MoveDirection){
			case 1:
				if(!eatFood) LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEAD_left);
				else {eatFood=0; LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEADOPEN_left);}
				break;	
			case 2:
				if(!eatFood) LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEAD_right);
				else {eatFood=0; LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEADOPEN_right);}
				break;		
			case 3:
				if(!eatFood) LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEAD_up);
				else {eatFood=0; LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEADOPEN_up);}
				break;				
			case 4:
				if(!eatFood) LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEAD_down);
				else {eatFood=0; LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEADOPEN_down);}
				break;	
			default:
				if(!eatFood) LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEAD_down);
				else {eatFood=0; LCD_Image2LcdDrawBmp565Pic(row,column,gImage_HEADOPEN_down);}
				break;
		}		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
}
void showSnakeBody(uint16_t body)
{
		int row=((body>>8)& 0x00ff )*20;
		int column=( body & 0x00ff )*20;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		LCD_Image2LcdDrawBmp565Pic(row,column,gImage_body);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	
}
void showFood(uint8_t FoodCoordinate_x,uint8_t FoodCoordinate_y)
{
		int row=FoodCoordinate_x*20;
		int column=FoodCoordinate_y*20;
		//if(row>=220) row-=3;
		//if(column>=300) column-=3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		LCD_Image2LcdDrawBmp565Pic(row,column,gImage_food);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	
}
void showSnakeEnd(uint16_t end)
{
		int row=((end>>8)& 0x00ff )*20;
		int column=( end & 0x00ff )*20;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		switch(endJudge() ){
			case 1:
				LCD_Image2LcdDrawBmp565Pic(row,column,gImage_end_left);
				break;	
			case 2:
				LCD_Image2LcdDrawBmp565Pic(row,column,gImage_end_right);
				break;		
			case 3:
			LCD_Image2LcdDrawBmp565Pic(row,column,gImage_end_up);
				break;				
			case 4:
			LCD_Image2LcdDrawBmp565Pic(row,column,gImage_end_down);
				break;	
			default:
				LCD_Image2LcdDrawBmp565Pic(row,column,gImage_end_down);
				break;
		}		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
}
uint8_t endJudge(void)
{
	int x=SnakeCoordinate[SnakeLenth-1]>>8 & 0x00ff;
	int y=SnakeCoordinate[SnakeLenth-1] & 0x00ff;
	int prex=SnakeCoordinate[SnakeLenth-2]>>8 & 0x00ff;
	int prey=SnakeCoordinate[SnakeLenth-2] & 0x00ff;	
	if(x==prex && y>prey) return 1;
	if(x==prex && y<prey) return 2;
	if(x>prex && y==prey) return 3;	
	if(x<prex && y==prey) return 4;
		return 1;
}
void gameMain(void)
{
	int forward_x=0,forward_y=0;
	if(StartFlag!=GAMEING) return;
	switch(MoveDirection)
		{
			case UP :
			{
				forward_x=(SnakeCoordinate[0]>>8&0x00ff)-1;
				forward_y=(SnakeCoordinate[0]&0x00ff);	//��ȡ����ǰ������(x-1,y),����
			}break;
			
			case DOWN :
			{
				forward_x=(SnakeCoordinate[0]>>8&0x00ff)+1;
				forward_y=(SnakeCoordinate[0]&0x00ff);	//��ȡ����ǰ������(x+1,y),����
			}break;
			
			case LEFT :
			{
				forward_x=(SnakeCoordinate[0]>>8&0x00ff);
				forward_y=(SnakeCoordinate[0]&0x00ff)+1;	//��ȡ����ǰ������(x,y+1),����
			}break;
			
			case RIGHT :
			{
				forward_x=(SnakeCoordinate[0]>>8&0x00ff);
				forward_y=(SnakeCoordinate[0]&0x00ff)-1;	//��ȡ����ǰ������(x,y-1),����
			}break;
			
			default:
				break;
		}
/*-----------------------����ǰ������������������------------------------------------*/
		if(Check_Coordinate(forward_x,forward_y) == -1)
				StartFlag=GAMEOVER;	//��ײ��ǽ����������,��Ϸ����
		else
				if((forward_x == FoodCoordinate_x) && (forward_y == FoodCoordinate_y))
						Move_Snake(forward_x,forward_y,FOOD);	//�ƶ�����,��ǰ��Ϊʳ��
					else
						Move_Snake(forward_x,forward_y,NOFOOD);	//�ƶ�����,��ǰ�㲻Ϊʳ��		
		newShow();
		
}
void newShow(void)
{
	int i=0;

	if(StartFlag==GAMEOVER){	
		//StartFlag=GAMEOVER;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		LCD_Image2LcdDrawBmp565Pic(0,0,gImage_GAMEOVER);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		
	}
	else{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		ili9320_Clear(0x9EDD);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
		
		showSnakeHead(SnakeCoordinate[0]);
		for(i=1;i<SnakeLenth-1;i++)	showSnakeBody(SnakeCoordinate[i]);
		showSnakeEnd(SnakeCoordinate[i]);
		showFood(FoodCoordinate_x,FoodCoordinate_y);	//��ʾʳ��	
	}
}
/*---------------------------------------------------------------*/
int Check_Coordinate(int x,int y)
{
		if(x<0 || x>11 || y<0 || y>15)	return -1;	//���곬���߽�
		if( (DisplayBuffer[16*x+y]!=0) && (x != FoodCoordinate_x) && (y != FoodCoordinate_y) )
			return -1;	//�㲻Ϊ�գ����Ҹõ㲻��ʳ��
		else
			return 1;	//��ǰ�����
}
void Move_Snake(uint8_t x,uint8_t y,uint8_t state)
{
	uint16_t counter;
	uint16_t i=0;
	if(state==FOOD)	//���ʳ�����굽����
	{
		for(counter=SnakeLenth;counter>0;counter--)
			SnakeCoordinate[counter]=SnakeCoordinate[counter-1];	//����β���һ������,����ӳ�һλ,��������������һλ
		SnakeCoordinate[0]=(x<<8)|y;	//ͷ����ӵ�ǰ������		
		SnakeLenth++;	//���ȼ�1
		
		for(i=0;i<192;i++)	DisplayBuffer[i]=0;//����ȫ���
		for(i=0;SnakeCoordinate[i]!=0;i++)	DisplayBuffer[16*(SnakeCoordinate[i]>>8&0x00ff)+(SnakeCoordinate[i]&0x00ff)]=1;

		createFood();
		DisplayBuffer[16*FoodCoordinate_x+FoodCoordinate_y]=2;//�µ�ʳ��������Ϊ2
		eatFood=1;
	}
	if(state==NOFOOD)	//ֱ���ƶ�����
	{
		DisplayBuffer[16*(SnakeCoordinate[SnakeLenth-1]>>8&0x00ff)+(SnakeCoordinate[SnakeLenth-1]&0x00ff)]=0;//����β��
		for(counter=SnakeLenth-1;counter>0;counter--)
			SnakeCoordinate[counter]=SnakeCoordinate[counter-1];	//ɾ��β�ڵ�,��������������һλ
		SnakeCoordinate[0]=(x<<8)|y;	//ͷ����ӵ�ǰ������
		DisplayBuffer[16*(SnakeCoordinate[0]>>8&0x00ff)+(SnakeCoordinate[0]&0x00ff)]=1;
	}
}
/*-------------------------������½������ӵĲ���---------------------------*/
void snake_move()
{
		ili9320_Clear(0x9EDD);
		switch(MoveDirection){
			case 0:
				LCD_Image2LcdDrawBmp565Pic(100,8,gImage_food);
				break;
			case 1:
				LCD_Image2LcdDrawBmp565Pic(100,8,gImage_HEAD_left);
				break;	
			case 2:
				LCD_Image2LcdDrawBmp565Pic(100,8,gImage_HEAD_right);
				break;		
			case 3:
			LCD_Image2LcdDrawBmp565Pic(100,8,gImage_HEAD_up);
				break;				
			case 4:
			LCD_Image2LcdDrawBmp565Pic(100,8,gImage_HEAD_down);
				break;	
			default:
				LCD_Image2LcdDrawBmp565Pic(100,8,gImage_food);
				break;
		}
// 	
// 	
// 	snake_time++;
// 		ili9320_Clear(0x9EDD);
// 		for(body_time=snake_time;body_time<4+snake_time;body_time++)
// 		{ 
// 		   LCD_Image2LcdDrawBmp565Pic(20+20*body_time,10,gImage_body);
// 		}
// 		LCD_Image2LcdDrawBmp565Pic(10,40,gImage_food);
// 		LCD_Image2LcdDrawBmp565Pic(100+snake_time*20,8,gImage_HEAD_down);
// 		LCD_Image2LcdDrawBmp565Pic(snake_time*20,10,gImage_end_down);
// 		Delay(0x3ff);
// 	for(snake_time=0;snake_time<10;snake_time++)
// 	{  	
// 		ili9320_Clear(0x9EDD);
// 		for(body_time=snake_time;body_time<4+snake_time;body_time++)
// 		{ 
// 		   LCD_Image2LcdDrawBmp565Pic(20+20*body_time,10,gImage_body);
// 		}
// 		LCD_Image2LcdDrawBmp565Pic(10,40,gImage_food);
// 		LCD_Image2LcdDrawBmp565Pic(100+snake_time*20,8,gImage_HEAD_down);
// 		LCD_Image2LcdDrawBmp565Pic(snake_time*20,10,gImage_end_down);
// 		Delay(0x3ff);
// 	}
}
