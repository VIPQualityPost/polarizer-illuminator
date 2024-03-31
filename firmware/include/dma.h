#ifndef __DMA_H__
#define __DMA_H__

#include "stm32g4xx_hal.h"

extern DMA_HandleTypeDef hdma_adc1;

void init_dma(void);

#endif