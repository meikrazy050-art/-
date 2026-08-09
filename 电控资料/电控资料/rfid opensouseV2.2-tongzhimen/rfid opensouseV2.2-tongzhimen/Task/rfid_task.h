#ifndef __RFID_TASK_H

#define __RFID_TASK_H

#include "stm32f1xx_hal.h"

#define BROADCAST_AND_LED            1
#define NBROADCAST_BUT_LED           2
#define NBROADCAST_AND_NLED          3

typedef struct 
{
    uint8_t chinese_data[60];
    int32_t chinese_block_num;
    int32_t read_block;
    uint8_t wait_time;
    uint8_t wait_resend_times;
    uint32_t led_flash_counts;
    float led_close_counts;
}rfid_control_t;



#endif //__RFID_TASK_H
