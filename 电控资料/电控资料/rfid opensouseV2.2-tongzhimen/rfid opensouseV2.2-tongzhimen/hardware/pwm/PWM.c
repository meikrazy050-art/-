#include "PWM.h"
//////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 一点创绘淘宝店：https://ydch.taobao.com/category.htm?spm=a1z10.3-c.w4010-24012836213.2.470e1d56kXwnZr&search=y
// 技术交流群：497550646
// 作者：一点创绘-卢佳威
//////////////////////////////////////////

extern TIM_HandleTypeDef PWM_HAL_TIMX;


/**
***********************************************************************************************************
*	@Function		: PWM_Init
*	@Description	:	PWM初始化设置
*	@Input			:	None
*	@Return			: None
***********************************************************************************************************
*/
void PWM_Init( void )
{
  HAL_TIM_PWM_Start( &PWM_HAL_TIMX, PWM_CHANNELX );     //初始化通道1
	HAL_TIM_PWM_Start( &PWM_HAL_TIMX, PWM_CHANNELX2);     //初始化通道2
}

/**
***********************************************************************************************************
*	@Function		: PWM_DutySet
*	@Description	:	PWM占空比设置函数（其实是高电平时间设置函数，占空比是一个百分比）
*	@Input			:	
*         *timx : 定时器名字，如TIM1、TIM2、TIM3等等
*         channel : 定时器通道号，如1、2、3、4
*         duty : 设置的占空比，注意要在周期内
*	@Return			: None
***********************************************************************************************************
*/
void PWM_DutySet( TIM_TypeDef* timx, uint8_t channel, uint16_t duty )
{
  switch( channel )
  {
    case 1:
    {
      timx->CCR1 = duty;
      break;
    }
    case 2:
    {
      timx->CCR2 = duty;
      break;
    }
    case 3:
    {
      timx->CCR3 = duty;
      break;
    }
    case 4:
    {
      timx->CCR4 = duty;
      break;
    }
  }
}

// user define
void Motor_Control( uint16_t speed )
{
	uint16_t set_speed = speed;
	
	if( set_speed > 999 )	set_speed = 999;
	
	if( speed == 0 )
	{
		PWM_DutySet( PWM_TIMX, 1, 999 );
		PWM_DutySet( PWM_TIMX, 2, 999 );
	}
	else
	{
		PWM_DutySet( PWM_TIMX, 1, speed );
		PWM_DutySet( PWM_TIMX, 2, speed );
	}
}
  


