#include "rfid_task.h"
#include "user_setup.h"
#include "BSP_USART.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "Card.h"
#include "cmsis_os.h"

static void BufClear( uint8_t* buf );
static void RFID_Par_Init( void );
static uint8_t RFID_Broadcast_LED_Control( void );

rfid_control_t rfid_control;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t card_res;
extern uint8_t card_res_flag;
extern CMD Cmd;

/**
************************************************************************************************
* @brief    UID数据读取，LED指示灯点亮，语音播报等功能
* @param    None
* @return   None
* @author   一点创绘
************************************************************************************************
**/
void RFID_Task(void const * argument)
{
    /* USER CODE BEGIN RFID_Task */
    /* Infinite loop */
    vTaskDelay(500);    // 等待语音模块初始化
	  #if RFID_SETTING_SPEAK_SPEED
    // /************************** 语速设置 ********************************/
    printf("<S>3");     // 语音设置为速度3，默认速度2，语音播报速度加到最快
    vTaskDelay(500);    // 等待语音模块初始化
    /*******************************************************************/
	  #endif 
    RFID_Par_Init();
    
    for(;;)
    {
        if( card_res_flag == CARD_FLAG_EXIST )  // 识别到UID
        {
            LED_Sta( 1 );   // 打开LED
            BufClear( Cmd.block_data ); // 清空接收到的数据
            ReadBlock( rfid_control.read_block+rfid_control.chinese_block_num );    // 设置读取的UID区块
            while( !__HAL_UART_GET_FLAG( &CARD_HAL_USARTx, UART_FLAG_TC ) );	// 等待串口发送完成
            card_res_flag = CARD_FLAG_WAIT;     // 进入等待模式，等待接收 RFID 数据
            rfid_control.wait_time = 0;
            rfid_control.wait_resend_times = 0;
        }
        else if( card_res_flag == CARD_FLAG_RESDATA )	// 接收到RFID数据
        {
            if( rfid_control.chinese_block_num == 0 && !Cmd.block_data[0] )	// UID中无数据
            {
                // 第4区块无数据，则表明没有用官方APP录入UID
                // 重新读取UID的第1区块 rfid_control.read_block+rfid_control.chinese_block_num=4-3=1
                rfid_control.chinese_block_num = -3;
                card_res_flag = CARD_FLAG_EXIST;    // 再次读取UID数据
            }
            else if( rfid_control.chinese_block_num == -3 && !Cmd.block_data[0] )	// UID中无数据
            {
                // 第1和第4区块均无数据	
                card_res_flag = CARD_FLAG_NONE;     // UID内无数据，准备进入低功耗模式
                #if RFID_DEBUG_MODE
                    printf("UID内无数据");        // 语音播报，方便调试
                    while( !__HAL_UART_GET_FLAG( &huart1, UART_FLAG_TC ) );	    // 等待串口发送完成
                    vTaskDelay( 200 );
                #endif
            }
            else	// UID中有数据
            {
                if( rfid_control.chinese_block_num == -3 )   // 如果数据在UID区块1
                {
										//LED_Sta( 1 );   // 打开LED
									  Usartx_SendString( Cmd.block_data );    // 语音播报
									  /*if(rfid_control.chinese_data[0]==0xB7&&rfid_control.chinese_data[1]==0xC9)
										{
									  uint8_t tongzhimenchonga[] = { 0xCD, 0xAC, 0xD6, 0xBE, 0xC3, 0xC7, 0xB3, 0xE5, 0xB0, 0xA1 };
                    Usartx_SendString( tongzhimenchonga );
									  }*/
                    while( !__HAL_UART_GET_FLAG( &HAL_USARTX, UART_FLAG_TC ) );	    // 等待串口发送完成
                    vTaskDelay( 200 );
                    card_res_flag = CARD_FLAG_LEDLIGHT;
                    rfid_control.led_close_counts = RFID_LED_LATE_CLOSE_TIME*20;
                }
                else if( rfid_control.chinese_block_num > 0 )	// 如果使用官方APP录入UID
                {
										//LED_Sta( 1 );   // 打开LED
									  Usartx_SendString( rfid_control.chinese_data );    // 语音播报
                    /*if(rfid_control.chinese_data[0]==0xB7&&rfid_control.chinese_data[1]==0xC9)
										{
									  uint8_t tongzhimenchonga[] = { 0xCD, 0xAC, 0xD6, 0xBE, 0xC3, 0xC7, 0xB3, 0xE5, 0xB0, 0xA1 };
                    Usartx_SendString( tongzhimenchonga );
									  }*/
                    while( !__HAL_UART_GET_FLAG( &HAL_USARTX, UART_FLAG_TC ) );	    // 等待串口发送完成
                    vTaskDelay( 200 );
                    card_res_flag = CARD_FLAG_LEDLIGHT;
                    rfid_control.led_close_counts = RFID_LED_LATE_CLOSE_TIME*20;
                }
                else	// （这段不会进入，功能不会执行，为了加快UID数据的读取）读取UID5、6区块。使用官方APP录入UID时，若录入中文过长，会将多余中文存储在5、6区块中
                {
                    for( uint8_t i = 0; i < 16; i++ )
                    {
                        rfid_control.chinese_data[i+rfid_control.chinese_block_num*16] = Cmd.block_data[i];
                    }
                    rfid_control.chinese_block_num++;
                    card_res_flag = CARD_FLAG_EXIST;
                }
                
            }
        }
        else if( card_res_flag == CARD_FLAG_WAIT )		// 等待RFID模块数据
        {
            vTaskDelay( 1 );
            rfid_control.wait_time++;
            if( rfid_control.wait_time >= 20 )	// 等待时间超过20ms
            {
                rfid_control.wait_resend_times ++;
                card_res_flag = CARD_FLAG_EXIST;
            }
            if( rfid_control.wait_resend_times >= 2 )	// 等待次数超过2次
            {
                rfid_control.wait_resend_times = 0;
                rfid_control.wait_time = 0;
                card_res_flag = CARD_FLAG_NONE;
            }
        }
        
        if( card_res_flag == CARD_FLAG_NONE )		// 单片机不工作，等待识别到UID
        {
            BufClear( rfid_control.chinese_data );
            rfid_control.chinese_block_num = 0;
            LED_Sta( 0 );
            rfid_control.wait_time = 0;
            rfid_control.wait_resend_times = 0;
        }
        else
        {
            if( card_res_flag == CARD_FLAG_LEDLIGHT )   // 语音播报结束后，检测UID是否一直在线圈内
            {
                LED_Sta( 1 );
                ReadCard();
                vTaskDelay( 10 );
            }
        }

        vTaskDelay(1);
    }
    
  /* USER CODE END RFID_Task */
}

/**
************************************************************************************************
* @brief    参数初始化
* @param    None
* @return   None
* @author   一点创绘
************************************************************************************************
**/
static void RFID_Par_Init( void )
{
    rfid_control.chinese_block_num = 0;
    rfid_control.read_block = 4;
    rfid_control.wait_time = 0;
    rfid_control.wait_resend_times = 0;
    rfid_control.led_flash_counts = 0;
    rfid_control.led_close_counts = 0;

    #if RFID_READ_DATA_WHEN_START
        card_res_flag = CARD_FLAG_EXIST;
    #else
        card_res_flag = CARD_FLAG_NONE;
    #endif
}

/**
************************************************************************************************
* @brief    语音播报和LED控制，根据宏定义选择播报和亮灯哪个点（供大家自己写）
* @param    None
* @return   None
* @author   一点创绘
************************************************************************************************
**/
static uint8_t RFID_Broadcast_LED_Control( void )
{
    
}

/**
************************************************************************************************
* @brief    清空数组
* @param    buf: 输入要清空的数组
* @return   None
* @author   一点创绘
************************************************************************************************
**/
static void BufClear( uint8_t* buf )
{
	uint8_t k = 0;
	
	while( buf[k] != '\0' )
	{
		buf[k] = 0;
		k++;
	}
}

