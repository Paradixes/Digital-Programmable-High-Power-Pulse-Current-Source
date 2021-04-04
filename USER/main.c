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
 ALIENTEK STM32������STemWinʵ��
 STemWin�޲���ϵͳ��ֲʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
    Write_Through();                //Cahceǿ��͸д
    MPU_Memory_Protection();        //������ش洢����
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(160,5,2,4);    //����ʱ��,400Mhz 
    delay_init(400);			    //��ʱ��ʼ��
    uart_init(115200);			    //���ڳ�ʼ��
    LED_Init();					    //��ʼ��LED
    KEY_Init();					    //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    TFTLCD_Init();				    //��ʼ��LCD
    tp_dev.init();				    //��������ʼ�� 
    TIM3_Init(999,200-1);           //1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,2000-1);          //������ɨ���ٶ�,100HZ.
    my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
    my_mem_init(SRAMDTCM);          //��ʼ���ڲ�DTCM�ڴ��
    __HAL_RCC_CRC_CLK_ENABLE();		//ʹ��CRCʱ��
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);//����STemWin�洢�豸
    WM_MULTIBUF_Enable(1);			//����STemWin�໺�壬RGB�����ܻ��õ�
    MainTask();					//STemWin ��ʾdemo
    while(1)
    {
    } 
}

