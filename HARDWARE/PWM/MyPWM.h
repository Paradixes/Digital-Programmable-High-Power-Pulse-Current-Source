#ifndef MYPWM_H
#define MYPWM_H

#define CH1_PIN GPIO_Pin_7
#define CH1_PORT GPIOA
#define CH1_SOURCE GPIO_PinSource7

#define CH1N_PIN GPIO_Pin_8
#define CH1N_PORT GPIOA
#define CH1N_SOURCE GPIO_PinSource8

#define TIM1_CLK RCC_AHB1Periph_GPIOA
void HalPWMGPIO_Config(void);
void HalTIM1_Config(unsigned int arr, unsigned int psc);
void TIM_Init(unsigned int arr, unsigned int psc);
void TIM5_CH1_Init(unsigned int arr, unsigned int psc);
void TIM3_CH1_Init(unsigned int arr, unsigned int psc);
void TIM2_CH1_Init(unsigned int arr, unsigned int psc);
void TIM4_CH1_Init(unsigned int arr, unsigned int psc);

#endif
