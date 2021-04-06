#include "UserDefine.h"
#include "stm32h7xx.h"

//? What does to this
#include "core_cm7.h"

void delay(__IO uint32_t nCount)
{
    __IO uint32_t index = 0;
    for (index = (84 * nCount); index != 0; index--)
    {
    }
}

void SEL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //使能GPIOG时钟

    //PG13、PG14和PG15初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //LED0、LED1和LED2对应IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                          //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);                                 //初始化GPIO

    GPIO_SetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //GPIOG13,G14,G15设置高，灯灭
}

void DA_CS(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CS_PORT, DA_CS_PIN);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CS_PORT, DA_CS_PIN);
    }
}

void DA_CS2(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CS_PORT, DA_CS_PIN2);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CS_PORT, DA_CS_PIN2);
    }
}

void DA_CS3(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CS_PORT, DA_CS_PIN3);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CS_PORT, DA_CS_PIN3);
    }
}

void DA_CLK(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CLK_PORT, DA_CLK_PIN);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CLK_PORT, DA_CLK_PIN);
    }
}

void DA_CLK2(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CLK_PORT, DA_CLK_PIN2);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CLK_PORT, DA_CLK_PIN2);
    }
}

void DA_CLK3(u16 status)
{
    if (status == 1)
        GPIO_SetBits(DA_CLK_PORT, DA_CLK_PIN3);
    else if (status == 0)
    {
        GPIO_ResetBits(DA_CLK_PORT, DA_CLK_PIN3);
    }
}

///////////////////////////////////////////////write data to DAC B and BUFFER
void Output_b(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;
    value = value + 1024 * 16;
    data = value;

    DA_CS(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK(1);
        if (temp != 0)
            GPIO_SetBits(DA_DIN_PORT, DA_DIN_PIN);
        else
            GPIO_ResetBits(DA_DIN_PORT, DA_DIN_PIN);
        delay(1);
        DA_CLK(0);
        delay(1);
    }

    GPIO_SetBits(DA_DIN_PORT, DA_CLK_PIN);
    delay(1);
    DA_CS(1);
    delay(1);
}

//write data to BUFFER
//DIN15-DIN12:Program Bits
//DIN11=DIN2:Data
//DIN1-DIN0:Input Word
void Output_a(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //左移2位
    value = value + 36864; //36864->9000H
    data = value;

    DA_CS(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK(0);
        if (temp != 0)
            GPIO_SetBits(DA_DIN_PORT, DA_DIN_PIN);
        else
            GPIO_ResetBits(DA_DIN_PORT, DA_DIN_PIN);
        delay(1);
        DA_CLK(1);
        delay(1);
    } //DIN输入控制字

    GPIO_SetBits(DA_DIN_PORT, DA_CLK_PIN);
    delay(1);
    DA_CS(1);
    delay(1);
}

//////////////////////////////////////////write data to DAC A and update DAC B with BUFFER content
void Output_c(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //10001111 --> 00111100  60
    value = value + 40960; //A000H:
    data = value;
    DA_CS(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK(0);
        if (temp != 0)
            GPIO_SetBits(DA_DIN_PORT, DA_DIN_PIN);
        else
            GPIO_ResetBits(DA_DIN_PORT, DA_DIN_PIN);
        delay(1);
        DA_CLK(1);
        delay(1);
    }

    GPIO_SetBits(DA_DIN_PORT, DA_CLK_PIN);
    delay(1);
    DA_CS(1);
    delay(1);
}

void Output_a2(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //左移2位
    value = value + 36864; //36864->9000H
    data = value;

    DA_CS2(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK2(0);
        if (temp != 0)
            GPIO_SetBits(DA_CLK_PORT, DA_DIN_PIN2);
        else
            GPIO_ResetBits(DA_CLK_PORT, DA_DIN_PIN2);
        delay(1);
        DA_CLK2(1);
        delay(1);
    } //DIN输入控制字

    GPIO_SetBits(DA_CS_PORT, DA_CLK_PIN2);
    delay(1);
    DA_CS2(1);
    delay(1);
}

void Output_c2(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //10001111 --> 00111100  60
    value = value + 40960; //A000H:
    data = value;
    DA_CS2(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK2(0);
        if (temp != 0)
            GPIO_SetBits(DA_CLK_PORT, DA_DIN_PIN2);
        else
            GPIO_ResetBits(DA_CLK_PORT, DA_DIN_PIN2);
        delay(1);
        DA_CLK2(1);
        delay(1);
    }

    GPIO_SetBits(DA_CS_PORT, DA_CLK_PIN2);
    delay(1);
    DA_CS2(1);
    delay(1);
}

void Output_a3(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //左移2位
    value = value + 36864; //36864->9000H
    data = value;

    DA_CS3(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK3(0);
        if (temp != 0)
            GPIO_SetBits(DA_CLK_PORT, DA_DIN_PIN3);
        else
            GPIO_ResetBits(DA_CLK_PORT, DA_DIN_PIN3);
        delay(1);
        DA_CLK3(1);
        delay(1);
    } //DIN输入控制字

    GPIO_SetBits(DA_CS_PORT, DA_CLK_PIN3);
    delay(1);
    DA_CS3(1);
    delay(1);
}

void Output_c3(u32 value)
{
    uint32_t data;
    u16 temp = 0;
    u8 i = 0;
    value = value << 2;    //10001111 --> 00111100  60
    value = value + 40960; //A000H:
    data = value;
    DA_CS3(0); //CS置低电平
    delay(1);

    for (; i <= 15; i++)
    {
        temp = data & 0x8000;
        data = data << 1;
        DA_CLK3(0);
        if (temp != 0)
            GPIO_SetBits(DA_CLK_PORT, DA_DIN_PIN3);
        else
            GPIO_ResetBits(DA_CLK_PORT, DA_DIN_PIN3);
        delay(1);
        DA_CLK3(1);
        delay(1);
    }

    GPIO_SetBits(DA_CS_PORT, DA_CLK_PIN3);
    delay(1);
    DA_CS3(1);
    delay(1);
}
//void Output_d(u32 value)
//{
//
//}
