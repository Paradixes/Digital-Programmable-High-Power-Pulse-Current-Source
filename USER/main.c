// Device headers
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_i2c.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_spi.h"

// Headers
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "sdram.h"
#include "mpu.h"
#include "timer.h"
#include "malloc.h"
#include "usmart.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "spi.h"

//transplant headers
// #include "MyPWM.h"
// #include "common.h"
// #include "misc.h"
// #include "miic.h"
// #include "tcs34725.h"


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

//GUI MainTask
#include "Magic.h"

SPI_HandleTypeDef hspi1;

uint8_t test[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

int main(void)
{
    Write_Through();                //Cahce强制透写
    MPU_Memory_Protection();        //保护相关存储区域
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();                     //初始化HAL库
    Stm32_Clock_Init(160, 5, 2, 4); //设置时钟,400Mhz
    delay_init(400);                //延时初始化
    uart_init(115200);              //串口初始化
    LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    TFTLCD_Init();                  //初始化LCD
    tp_dev.init();                  //触摸屏初始化
    TIM3_Init(999, 200 - 1);        //1KHZ 定时器3设置为1ms
    TIM4_Init(999, 2000 - 1);       //触摸屏扫描速度,100HZ
    my_mem_init(SRAMIN);            //初始化内部内存池
    my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
    my_mem_init(SRAMDTCM);          //初始化内部DTCM内存池
    MX_GPIO_Init();                 //GPIO output PA4 
    MX_SPI1_Init();                 //SPI1SCK PA5, SPI1MISO PA6, SPI1MOSI PA7
    __HAL_RCC_CRC_CLK_ENABLE(); 		//使能CRC时钟
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV); //开启STemWin存储设备
    WM_MULTIBUF_Enable(1);           //开启STemWin多缓冲，RGB屏可能会用到
    //GUIMainTask();                      //STemWin 主函数

		

    while (1)
    {
        HAL_SPI_MspInit(&hspi1);
        //GUI_Delay(100);
        delay_ms(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

        HAL_SPI_Transmit(&hspi1, test, 6, 500);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    }
}
