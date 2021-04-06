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

void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#endif
