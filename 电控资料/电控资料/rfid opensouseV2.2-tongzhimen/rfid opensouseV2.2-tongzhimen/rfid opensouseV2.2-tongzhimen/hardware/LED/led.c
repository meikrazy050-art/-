#include "led.h"
//#include "task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"

/**
************************************************************************************************
* @brief    控制LED灯电平反转。（三个引脚的LED灯，针对不同需要且三个引脚兼容，不与硬件起冲突。可以不修改或加上自己需要的引脚）
* @param    None
* @return   None
* @author   一点创绘——卢佳威
************************************************************************************************
**/
void LED_Toggle( void )
{
    HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_13 );    // 电平翻转
    HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_12 );
    HAL_GPIO_TogglePin( GPIOA, GPIO_PIN_8 );
}

/**
************************************************************************************************
* @brief    控制LED灯亮或者灭。（三个引脚的LED灯，针对不同需要且三个引脚兼容，不与硬件起冲突。可以不修改或加上自己需要的引脚）
* @param    sta: 控制LED灯的状态，当为 1 时灯亮，当为 0 时灯灭，其他值不做响应
* @return   None
* @author   一点创绘——卢佳威
************************************************************************************************
**/
void LED_Sta( uint8_t sta )
{
		//vTaskDelay(500);
	 // HAL_Delay(1000);
    switch( sta )
    {
        case 1:     //灯亮
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET );     // GPIO 置0
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_12, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8, GPIO_PIN_RESET );
            break;
        }
        case 0:     //灯灭
        {
            HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, GPIO_PIN_SET );       // GPIO 置1
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_12, GPIO_PIN_SET );
            HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8, GPIO_PIN_SET );
            break;
        }
        default:
        {
            break;
        }
	}
				
}



