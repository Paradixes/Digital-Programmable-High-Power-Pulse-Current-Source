#ifndef __SPI_H
#define __SPI_H

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
//#include "stm32h7xx_hal_it.h"

extern SPI_HandleTypeDef hspi1;

void MX_SPI1_Init(void);
void MX_GPIO_Init(void);

#endif
