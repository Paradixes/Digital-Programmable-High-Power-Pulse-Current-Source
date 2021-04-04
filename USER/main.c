#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "sdram.h"
#include "mpu.h"
#include "timer.h"
#include "usmart.h"
#include "malloc.h"
#include "touch.h"
#include "usmart.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "Magic.h"
/************************************************
 ALIENTEK STM32开发板STemWin实验
 STemWin无操作系统移植实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
    Write_Through();                //Cahce强制透写
    MPU_Memory_Protection();        //保护相关存储区域
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(160,5,2,4);    //设置时钟,400Mhz 
    delay_init(400);			    //延时初始化
    uart_init(115200);			    //串口初始化
    LED_Init();					    //初始化LED
    KEY_Init();					    //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    TFTLCD_Init();				    //初始化LCD
    tp_dev.init();				    //触摸屏初始化 
    TIM3_Init(999,200-1);           //1KHZ 定时器3设置为1ms
    TIM4_Init(999,2000-1);          //触摸屏扫描速度,100HZ.
    my_mem_init(SRAMIN);            //初始化内部内存池
    my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
    my_mem_init(SRAMDTCM);          //初始化内部DTCM内存池
    __HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);//开启STemWin存储设备
    WM_MULTIBUF_Enable(1);			//开启STemWin多缓冲，RGB屏可能会用到
    MainTask();					//STemWin 演示demo
    while(1)
    {
    } 
}

