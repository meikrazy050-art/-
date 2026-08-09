#ifndef __BSP_USART_H

#define __BSP_USART_H

//////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 一点创绘淘宝店：https://ydch.taobao.com/category.htm?spm=a1z10.3-c.w4010-24012836213.2.470e1d56kXwnZr&search=y
// 技术交流群：497550646
// 作者：一点创绘-卢佳威
//////////////////////////////////////////

#include "stm32f1xx_hal.h"


/********** 串口号设置 ************/
#define HAL_USARTX            huart2
#define USARTX                USART2



void Usartx_SendString( uint8_t *str );
void Usartx_RxIT_Start( uint16_t rx_num );


#endif
