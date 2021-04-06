#include "stm32h7xx.h"
#include "common.h"

#define SETB 1

#define DA_CLK_PIN GPIO_Pin_1
#define DA_CS_PIN GPIO_Pin_2
#define DA_DIN_PIN GPIO_Pin_3

#define DA_CLK_PIN2 GPIO_Pin_11
#define DA_CS_PIN2 GPIO_Pin_12
#define DA_DIN_PIN2 GPIO_Pin_8

#define DA_CLK_PIN3 GPIO_Pin_13
#define DA_CS_PIN3 GPIO_Pin_14
#define DA_DIN_PIN3 GPIO_Pin_15

//#define DA_TEST_H      GPIO_Pin_4
//#define DA_TEST_L      GPIO_Pin_5

#define DA_CS_PORT GPIOA
#define DA_CLK_PORT GPIOA
#define DA_DIN_PORT GPIOA

#define SEL1 PGout(13)
#define SEL2 PGout(14)
#define SEL3 PGout(15)
void SEL_Init(void); //初始化

//#define DA_CH2_PORT       GPIOB

//应用函数
void DA_GPIO_Init(void); //初始化三个引脚
void Output_a(u32 value);
void Output_b(u32 value); //设置B的数据
void Output_a2(u32 value);
void Output_c2(u32 value); //设置B的数据
void Output_a3(u32 value);
void Output_c3(u32 value);
void Output_c(u32 value);
void Output_d(u32 value);
void DA_CS(u16 status); // 设置CS的值
void DA_CLK(u16 status);
void DA_CS2(u16 status); // 设置CS的值
void DA_CLK2(u16 status);
void DA_CS3(u16 status); // 设置CS的值
void DA_CLK3(u16 status);
void delay(__IO uint32_t nCount);
