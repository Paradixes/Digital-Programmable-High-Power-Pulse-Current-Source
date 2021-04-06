#ifndef __MIIC_H_
#define __MIIC_H_

#include "sys.h"
#include "stm32f4xx.h"
#define MIICSDAGPIO		GPIOB
#define MIICSCLGPIO		GPIOB
#define MIICCLOCKCMD	RCC_AHB1Periph_GPIOB
#define MIICSDAPIN		GPIO_Pin_14
#define MIICSCLPIN		GPIO_Pin_15

#define IIC_SDA_1			GPIO_SetBits(MIICSDAGPIO,MIICSDAPIN)
#define IIC_SDA_0			GPIO_ResetBits(MIICSDAGPIO,MIICSDAPIN)
#define IIC_SCL_1			GPIO_SetBits(MIICSCLGPIO,MIICSCLPIN)
#define IIC_SCL_0			GPIO_ResetBits(MIICSCLGPIO,MIICSCLPIN)

#define SDA_READ			GPIO_ReadInputDataBit(MIICSDAGPIO, MIICSDAPIN)

void SDA_IN(void);
void SDA_OUT(void);

void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#endif
