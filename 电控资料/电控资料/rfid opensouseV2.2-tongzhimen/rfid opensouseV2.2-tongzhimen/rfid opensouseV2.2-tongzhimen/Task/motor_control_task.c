#include "motor_control_task.h"
#include "user_setup.h"
#include "BSP_ADC.h"
#include "PWM.h"
#include "cmsis_os.h"

/*#include "led.h"
//#include "task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"
*/
motor_control_t motor_control;

/**
************************************************************************************************
* @brief    电机控制任务，跟电机控制：晚启动，缓启动，电机定时停止等都在这里进行
* @param    None
* @return   None
* @author   一点创绘
************************************************************************************************
**/
void Motor_Control_Task(void const * argument)
{
    /* USER CODE BEGIN Motor_Control_Task */
    /* Infinite loop */
    // 读取ADC数据
	uint32_t adc_value = 0;
	for( uint8_t i = 0; i < 20; i++ )
	{
		adc_value += Get_ADC_Value();
		vTaskDelay( 1 );
	}
	adc_value = adc_value/20;
	motor_control.res = adc_value >= 4095 ? 5000-5000 : 5000-adc_value*1000/(4095-adc_value);	// 计算电位器电阻

    // 计算电机自动停止时间
     motor_control.stop_time = motor_control.res > RES_MAX ? STOP_TIME_MAX : STOP_TIME_MIN+motor_control.res*(STOP_TIME_MAX-STOP_TIME_MIN)/RES_MAX; 

    // 晚启动等待时间
    //vTaskDelay( MOTOR_START_LATE_TIME_SETUP );

    for(;;)
    {  
        motor_control.count_ms++;  // 自加一用来计数，单位 ms
         
			   if( motor_control.count_ms < MOTOR_START_SLOW_TIME_SETUP )
         {
             Motor_Control(350*motor_control.count_ms/MOTOR_START_SLOW_TIME_SETUP );//350*motor_control.count_msMOTOR_START_SLOW_TIME_SETUP 
         }
			   //else if(motor_control.count_ms>30000&&motor_control.count_ms<=40000)
				 //{
					  // Motor_Control(800);
				 //}
				 /*else if(motor_control.count_ms>5000&&motor_control.count_ms<=10000)
				 {
					 Motor_Control(500);
				 }
				 else if(motor_control.count_ms>25000&&motor_control.count_ms<=40000)
				 {
					 Motor_Control(800);
				 }*/
        else
        {
            Motor_Control(350);
					  //LED_Sta(1);
        }
			
        if( motor_control.count_ms > motor_control.stop_time )
        {
            Motor_Control( 800 );     // 关闭电机
        }
        
        if( motor_control.count_ms >= 1000*1000 )   // 当计数到 1000s 时，直接关闭电机
        {
            motor_control.count_ms = 1000*1000;
            Motor_Control( 800 );     // 关闭电机
        }
				vTaskDelay( 1 );

    }
    /* USER CODE END Motor_Control_Task */
}

