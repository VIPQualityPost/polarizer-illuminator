#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"

extern ADC_HandleTypeDef hadc1;

void init_adc(void);
void gpio_config_adc(ADC_HandleTypeDef*);

#ifdef __cplusplus
}
#endif

#endif