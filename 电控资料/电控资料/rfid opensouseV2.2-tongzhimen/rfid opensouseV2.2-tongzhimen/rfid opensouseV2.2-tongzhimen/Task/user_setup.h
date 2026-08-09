#ifndef __USER_SETUP_H


#define __USER_SETUP_H

/************************** 用户设置 ****************************/

//----------------------------------------------------------------
// 晚启动部分
#define MOTOR_START_LATE_TIME_SETUP                 0        // 500ms

// 缓启动部分
#define MOTOR_START_SLOW_TIME_SETUP                 1000  // 1000ms

// 电机定时停止
#define RES_MAX                                     5000        // 5K
#define STOP_TIME_MIN                               (10*1000)	  // 10s
#define STOP_TIME_MAX                               (10*60*1000)// 600s = 10min

//----------------------------------------------------------------
// RFID和语音播报部分
#define RFID_DEBUG_MODE                              0          // debug模式，1开启，0关闭，开启会语音播报相关错误，建议比赛时关闭
#define RFID_LED_LATE_CLOSE_TIME                     0      // LED延迟关闭时间设置，大约0.5s
#define RFID_READ_DATA_WHEN_START                    0          // 开机读取UID，1开启，0关闭
#define RFID_SETTING_SPEAK_SPEED                     0          // 开机自动设置语音加速，1开启，0关闭（缓启动板开启此功能，其他板子不建议）

//----------------------------------------------------------------

#endif //__USER_SETUP_H
