#ifndef __OPAMP_H__
#define __OPAMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_opamp.h"

extern OPAMP_HandleTypeDef hopamp1;

void init_opa(void);
void gpio_config_opa(OPAMP_HandleTypeDef*);

#ifdef __cplusplus
}
#endif

#endif