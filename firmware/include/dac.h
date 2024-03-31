#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_dac.h"

extern DAC_HandleTypeDef hdac3;

void init_dac(void);

#ifdef __cplusplus
}
#endif

#endif