#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "ARMJISHU_TouchScreen_ADS7843.h"
#include "picture.h"
#include "food.h"
#include "SNAKE.h"
#include "BODY.h"
#include "GAMEOVER.h"
#define NONE					0
#define LEFT					1
#define RIGHT					2
#define UP						3
#define DOWN					4
#define GAMEOVER			1
#define GAMEWAITING   2
#define GAMEING				3
#define GAMEPAUSE			4
#define FOOD	1
#define NOFOOD 0


typedef struct
{	
	u8 Snake_X;
	u8 Snake_Y;
	u8 Snake_Type;    //��ͷ��1������2����β��3  
}Snake_In[200];

u8 DisplayBuffer[192]={0};	//��ʾ����,����Ϊ[0,0],����Ϊ[11,15]
//0:�գ�1���ߣ�2��ʳ��
u8 StartFlag=GAMEOVER;	//��Ϸ��ʼ��־
uint8_t MoveDirection=DOWN;	//�ƶ�����
uint8_t preMoveDirection=DOWN;	//�ƶ�����
uint8_t FoodCoordinate_x,FoodCoordinate_y;	//ʳ������
uint16_t SnakeLenth;	//������
uint16_t SnakeCoordinate[192];	//��������,�߰�λ��x������,��8λ��y������
u8 eatFood=0;
u8 snake_time=0,body_time=0;
uint32_t seed=0;

void gamePicInit(void);
void snake_move(void);
void Init_Snake(void);
void showSnakeHead(uint16_t head);
void showSnakeBody(uint16_t body);
void showSnakeEnd(uint16_t end);
void showFood(uint8_t FoodCoordinate_x,uint8_t FoodCoordinate_y);
void gameMain(void);
int Check_Coordinate(int x,int y);
void Move_Snake(uint8_t x,uint8_t y,uint8_t state);
void newShow(void);
uint8_t endJudge(void);
void createFood(void);
int myRand(void);