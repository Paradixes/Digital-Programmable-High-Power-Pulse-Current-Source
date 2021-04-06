#include "MyPWM.h"
#include "stm32h7xx.h"

TIM_ICInitTypeDef  TIM5_ICInitStructure;
//TIM5 -- H&L MODE
void TIM5_CH1_Init(unsigned int arr,unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ե�
	TIM_OC1Init(TIM5,&TIM_OCInitStructure);
	
//	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
	
	
}
// TIM3 -- PWM MODE
void TIM3_CH1_Init(unsigned int arr,unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //PA0����λ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ե�
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	
//	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
	
	
}

// Channel2 H&L
void TIM2_CH1_Init(unsigned int arr,unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2); //PA0����λ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ե�
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
//	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	
}

void TIM4_CH1_Init(unsigned int arr,unsigned int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTAʱ��	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //PA0����λ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA0

	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��PWMģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ե�
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	
//	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM4,ENABLE);
	
	
}


//void HalPWMGPIO_Config(){
//	GPIO_InitTypeDef GPIO_InitStructure;
////	RCC_AHB1PeriphClockCmd(TIM1_CLK,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	GPIO_PinAFConfig(CH1_PORT, CH1_SOURCE, GPIO_AF_TIM1);   
//  GPIO_PinAFConfig(CH1N_PORT, CH1N_SOURCE, GPIO_AF_TIM1);
//	
//	//��������
// //TIM1_CH1N
//  GPIO_InitStructure.GPIO_Pin=CH1_PIN;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
//  GPIO_Init(CH1N_PORT,&GPIO_InitStructure);
// 
//  //TIM1_CH1
//  GPIO_InitStructure.GPIO_Pin=CH1N_PIN;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
//  GPIO_Init(CH1_PORT,&GPIO_InitStructure);
//	
//	
//}

//void HalTIM1_Config(unsigned int arr,unsigned int psc){
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	TIM_DeInit(TIM1);
//	
//	 //��ʱ����������
//  TIM_TimeBaseStructure.TIM_Period=arr;                          //arr �Զ���װ��ֵ
//  TIM_TimeBaseStructure.TIM_Prescaler=psc;                       //Ԥ��Ƶ,����1��Ƶ
//  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;          //��������Ƶ�й�
//  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
////  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;                 //�ƺ�û��
//  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

//  //��ʱ���Ƚ��������
//  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; 
////  TIM_OCInitStructure.TIM_Pulse=3000;                             //��ռ�ձȵ�ֵ
//  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;     
////  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;     //ָ������״̬�µ�TIM����Ƚϵ�����״̬��
//  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //�Ƚ����ʹ��
//  //�߼���ʱ�����еĻ����������
////  TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
////  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;  //�Ƚϻ������ʹ��
////  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; 
////  TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
//  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//	
//	
//  //�Զ���װ��
////  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);


////  //�߼���ʱ����������
////  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;            //
////  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;            //
////  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;               //
////  TIM_BDTRInitStructure.TIM_DeadTime = 0xA8;                             //2.6ns
////  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                   //
////  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;      //
////  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//
////  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);                          // 

//// 
////  TIM_ARRPreloadConfig(TIM1, ENABLE);
////	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
////	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
////  TIM_CtrlPWMOutputs(TIM1, ENABLE);             //�߼���ʱ�����е������ʹ��
////	
//	TIM_Cmd(TIM1,ENABLE);
////	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
////	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);	


//}

