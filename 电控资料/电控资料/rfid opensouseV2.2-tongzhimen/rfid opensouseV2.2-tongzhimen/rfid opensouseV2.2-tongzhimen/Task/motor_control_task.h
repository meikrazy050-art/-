#ifndef __MOTOR_CONTROL_TASK_H

#define __MOTOR_CONTROL_TASK_H

#include "stm32f1xx_hal.h"


typedef struct
{
    uint32_t res;
    uint32_t count_ms;
    uint32_t stop_time;
}motor_control_t;





#endif //__MOTOR_CONTROL_TASK_H
