#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////

//STM32按键驱动
//KEY0 在PC13口，按下接地（低电平有效）

//////////////////////////////////////////////////////////////////////////////////

//按键初始化函数
void KEY_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->PC13 输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PC时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PC13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化PC
}

//按键扫描函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0,没有任何按键按下

static u8 key_up=1;//按键按松开标志

u8 KEY_Scan(u8 mode)
{
	if(mode)key_up=1;  //支持连续按
	if(key_up&&(KEY1==0))  //检测低电平（按键按下接地）
	{
		delay_ms(10);//去抖动
		key_up=0;
		if(KEY1==0) return 1;
	}else if(KEY1==1)key_up=1;
 	return 0;// 无按键按下
}