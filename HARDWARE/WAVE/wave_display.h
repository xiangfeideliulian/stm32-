#ifndef __WAVE_DISPLAY_H
#define __WAVE_DISPLAY_H
#include "sys.h"
#include "key.h"

//横屏模式下的坐标轴参数 (屏幕尺寸160x128)
#define AXIS_LEFT      20     //坐标轴左边距
#define AXIS_TOP       5      //坐标轴上边距
#define AXIS_BOTTOM    110    //坐标轴底部位置（横轴）
#define AXIS_HEIGHT    105    //纵轴高度

#define WAVE_WIDTH     130    //波形宽度区域
#define WAVE_POINTS    120    //波形点数

void Draw_Axis(void);
void Clear_Wave(void);
void Update_Wave(void);
void Wave_Display_Loop(void);

#endif