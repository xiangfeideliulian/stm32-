#include "wave_display.h"
#include "lcd.h"
#include "adc.h"
#include "delay.h"
#include "key.h"

//波形数据缓冲区
static u16 wave_y[WAVE_POINTS];
static u8 wave_index = 0;

//绘制坐标轴
void Draw_Axis(void)
{
	u16 i;
	u16 y_pos;

	//绘制横轴 - 时间t轴 (底部，白色线条)
	LCD_DrawLine(AXIS_LEFT, AXIS_BOTTOM, AXIS_LEFT + WAVE_WIDTH, AXIS_BOTTOM, WHITE);

	//绘制纵轴 - 电压u轴 (左侧，白色线条)
	LCD_DrawLine(AXIS_LEFT, AXIS_TOP, AXIS_LEFT, AXIS_BOTTOM, WHITE);

	//横轴刻度标注：0, 30, 60, 90, 120 (使用12号字体)
	LCD_ShowString(AXIS_LEFT - 5, AXIS_BOTTOM + 3, 16, 12, 12, WHITE, "0");
	LCD_ShowString(AXIS_LEFT + 25, AXIS_BOTTOM + 3, 16, 12, 12, WHITE, "30");
	LCD_ShowString(AXIS_LEFT + 55, AXIS_BOTTOM + 3, 16, 12, 12, WHITE, "60");
	LCD_ShowString(AXIS_LEFT + 85, AXIS_BOTTOM + 3, 16, 12, 12, WHITE, "90");
	LCD_ShowString(AXIS_LEFT + 115, AXIS_BOTTOM + 3, 20, 12, 12, WHITE, "120");

	//横轴标签 "t/s"
	LCD_ShowString(AXIS_LEFT + WAVE_WIDTH + 2, AXIS_BOTTOM - 6, 16, 12, 12, WHITE, "t/s");

	//纵轴刻度标注：0V, 1V, 2V, 3V, 4V, 5V (使用12号字体)
	LCD_ShowString(0, AXIS_BOTTOM - 4, 16, 12, 12, WHITE, "0V");
	LCD_ShowString(0, AXIS_TOP + AXIS_HEIGHT * 4 / 5 - 4, 16, 12, 12, WHITE, "1V");
	LCD_ShowString(0, AXIS_TOP + AXIS_HEIGHT * 3 / 5 - 4, 16, 12, 12, WHITE, "2V");
	LCD_ShowString(0, AXIS_TOP + AXIS_HEIGHT * 2 / 5 - 4, 16, 12, 12, WHITE, "3V");
	LCD_ShowString(0, AXIS_TOP + AXIS_HEIGHT / 5 - 4, 16, 12, 12, WHITE, "4V");
	LCD_ShowString(0, AXIS_TOP - 4, 16, 12, 12, WHITE, "5V");

	//绘制中间刻度线（小横线）
	for(i = 1; i <= 4; i++)
	{
		y_pos = AXIS_TOP + i * AXIS_HEIGHT / 5;
		LCD_DrawLine(AXIS_LEFT - 3, y_pos, AXIS_LEFT, y_pos, WHITE);
	}

	//纵轴标签 "u" 放在顶部
	LCD_ShowString(0, 0, 16, 12, 12, WHITE, "u(V)");
}

//清除波形并重绘坐标轴
void Clear_Wave(void)
{
	u16 x, y;

	//清除波形区域的点（不包括坐标轴线）
	for(x = AXIS_LEFT + 1; x < AXIS_LEFT + WAVE_WIDTH; x++)
	{
		for(y = AXIS_TOP + 1; y < AXIS_BOTTOM; y++)
		{
			LCD_Fast_DrawPoint(x, y, BLACK);
		}
	}

	//重置索引
	wave_index = 0;

	//重新绘制坐标轴
	Draw_Axis();
}

//电压值转换为屏幕Y坐标
u16 Voltage_To_Y(float voltage)
{
	u16 y;

	//电压范围 0-3.3V，坐标轴是0-5V
	if(voltage > 3.3f) voltage = 3.3f;
	if(voltage < 0.0f) voltage = 0.0f;

	//Y坐标按5V坐标轴计算位置
	y = AXIS_BOTTOM - (u16)(voltage * AXIS_HEIGHT / 5.0f);

	//确保不超出绘图区域
	if(y < AXIS_TOP + 1) y = AXIS_TOP + 1;
	if(y > AXIS_BOTTOM - 1) y = AXIS_BOTTOM - 1;

	return y;
}

//更新波形显示（每0.2秒调用一次）
void Update_Wave(void)
{
	float voltage;
	u16 x_pos, y_pos;
	u16 prev_x, prev_y;

	//读取ADC实际电压值（0-3.3V）
	voltage = ADC_GetVoltage();

	//计算当前Y坐标
	y_pos = Voltage_To_Y(voltage);

	//保存Y坐标数据
	wave_y[wave_index] = y_pos;

	//计算当前X坐标（避开纵轴线）
	x_pos = AXIS_LEFT + wave_index + 1;

	//绘制当前点（红色）
	LCD_Fast_DrawPoint(x_pos, y_pos, RED);

	//与前一个点连线形成连续波形（红色）
	if(wave_index > 0)
	{
		prev_x = AXIS_LEFT + wave_index;
		prev_y = wave_y[wave_index - 1];
		LCD_DrawLine(prev_x, prev_y, x_pos, y_pos, RED);
	}

	//更新索引
	wave_index++;

	//达到120点后清除波形重新开始
	if(wave_index >= WAVE_POINTS)
	{
		Clear_Wave();
	}
}

//波形显示主循环
void Wave_Display_Loop(void)
{
	while(1)
	{
		Update_Wave();
		delay_ms(200); //每0.2秒采样一次

		//按键检测：按下清除波形重新开始
		if(KEY_Scan(1) == 1)
		{
			Clear_Wave();
		}
	}
}