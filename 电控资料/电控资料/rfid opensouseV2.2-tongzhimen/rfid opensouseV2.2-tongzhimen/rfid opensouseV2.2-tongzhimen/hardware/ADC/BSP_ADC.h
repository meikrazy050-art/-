#ifndef __BSP_ADC_H

#define __BSP_ADC_H

//////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 一点创绘淘宝店：https://ydch.taobao.com/category.htm?spm=a1z10.3-c.w4010-24012836213.2.470e1d56kXwnZr&search=y
// 技术交流群：497550646
// 作者：一点创绘-卢佳威
//////////////////////////////////////////

#include "stm32f1xx_hal.h"



/********** ADC 设置 ************/
#define HAL_ADCX              hadc1




void ADC_Init( void );
uint32_t Get_ADC_Value( void );

#endif 
