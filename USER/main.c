#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"
#include "Dis_Picture.h"
#include "Text.h"
#include "GBK_LibDrive.h"
#include "adc.h"
#include "wave_display.h"

//STM32F103核心板测试--ADC波形显示程序
//按键：PC13（按下接地）
//ADC：PA1（通道1）

int main(void)
{
	delay_init();	    	     //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组
	uart_init(115200);	     //串口初始化
	LED_Init();		         //LED端口初始化
	KEY_Init();              //按键初始化（PC13）

	LCD_Init();              //初始化LCD SPI接口

	GBK_Lib_Init();          //硬件GBK字库初始化

	POINT_COLOR=BLACK;       //设置画笔颜色
	BACK_COLOR=WHITE;        //设置背景颜色
	LCD_Clear(WHITE);        //清屏为白色

	//显示个人信息
	Show_Name();
	delay_ms(2000);          //显示2秒

	//等待按键触发进入波形显示模式
	while(KEY_Scan(1) != 1)
	{
		delay_ms(10);
	}

	//按键按下，进入波形显示模式
	LCD_Clear(BLACK);        //清屏为黑色（波形显示背景）
	BACK_COLOR = BLACK;      //设置背景颜色为黑色，确保文字显示正常

	My_ADC_Init();           //ADC初始化（PC1）

	Draw_Axis();             //绘制坐标轴

	//开始波形显示循环
	Wave_Display_Loop();

	while(1)
	{
		//主循环
	}
}