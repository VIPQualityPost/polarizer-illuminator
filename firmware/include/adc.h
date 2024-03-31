#ifndef __ADC_H__
#define __ADC_H__

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"
#include "stm32g4xx_hal_dma.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

void init_adc(void);
void gpio_config_adc(ADC_HandleTypeDef*);

#endif