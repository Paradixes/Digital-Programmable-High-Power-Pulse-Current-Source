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
    Write_Through();                //Cahceǿ��͸д
    MPU_Memory_Protection();        //������ش洢����
    Cache_Enable();                 //��L1-Cache
    HAL_Init();                     //��ʼ��HAL��
    Stm32_Clock_Init(160, 5, 2, 4); //����ʱ��,400Mhz
    delay_init(400);                //��ʱ��ʼ��
    uart_init(115200);              //���ڳ�ʼ��
    LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    TFTLCD_Init();                  //��ʼ��LCD
    tp_dev.init();                  //��������ʼ��
    TIM3_Init(999, 200 - 1);        //1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999, 2000 - 1);       //������ɨ���ٶ�,100HZ
    my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
    my_mem_init(SRAMDTCM);          //��ʼ���ڲ�DTCM�ڴ��
    MX_GPIO_Init();                 //GPIO output PA4 
    MX_SPI1_Init();                 //SPI1SCK PA5, SPI1MISO PA6, SPI1MOSI PA7
    __HAL_RCC_CRC_CLK_ENABLE(); 		//ʹ��CRCʱ��
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV); //����STemWin�洢�豸
    WM_MULTIBUF_Enable(1);           //����STemWin�໺�壬RGB�����ܻ��õ�
    //GUIMainTask();                      //STemWin ������

		

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
