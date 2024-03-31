#include <Arduino.h>
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "opa.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"
#include "stm32g4xx_hal_dac.h"
#include "stm32g4xx_hal_opamp.h"

#include "RTTStream.h"
#include "USBPowerDelivery.h"

#define USBD_MANUFACTURER_STRING "matei repair lab"
#define USBD_PRODUCT_STRING_FS "illuminator"

RTTStream rtt;

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac3;
extern OPAMP_HandleTypeDef hopamp1;

bool enabled = true;
uint16_t adc_data[2] = {0};

void userButton();

void setup() {
  pinMode(PB8, INPUT);
  pinMode(PA0, INPUT_ANALOG);
  pinMode(PA1, INPUT_ANALOG);
  pinMode(PA5, OUTPUT);
  pinMode(PA8, OUTPUT);
  pinMode(PA2, INPUT);

  attachInterrupt(PB8, userButton, RISING);

  PowerSink.start();
  PowerSink.requestPower(15000);

  init_dma();

  // init_adc();
  // gpio_config_adc(&hadc1);

  init_dac();
  __HAL_RCC_DAC3_CLK_ENABLE();

  init_opa();
  gpio_config_opa(&hopamp1);
  HAL_OPAMP_Start(&hopamp1);

  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 2);
  
  TIM_TypeDef *IlluminatorTimer = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA5), PinMap_PWM);
  uint32_t IlluminatorChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA5), PinMap_PWM));
  HardwareTimer* Illuminator = new HardwareTimer(IlluminatorTimer);
  Illuminator->setMode(IlluminatorChannel, TIMER_OUTPUT_COMPARE_PWM1, PA5);
  Illuminator->setOverflow(2000, HERTZ_FORMAT);
  Illuminator->setCaptureCompare(IlluminatorChannel, 90, HERTZ_COMPARE_FORMAT);

  TIM_TypeDef *PolarizerTimer = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA8), PinMap_PWM);
  uint32_t PolarizerChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA8), PinMap_PWM));
  HardwareTimer* Polarizer = new HardwareTimer(PolarizerTimer);
  Polarizer->setMode(PolarizerChannel, TIMER_OUTPUT_COMPARE_PWM1, PA8);
  Polarizer->setOverflow(1000, HERTZ_FORMAT);
  Polarizer->setCaptureCompare(PolarizerChannel, 50, PERCENT_COMPARE_FORMAT);

  Illuminator->resume();
  Polarizer->resume();
}

void loop(){ 
  adc_data[1] = analogRead(PA0) << 6;
  adc_data[0] = analogRead(PA1) << 6;

  TIM2->CCR1 = adc_data[0];
  HAL_DAC_SetValue(&hdac3, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adc_data[1]);
}

void userButton(void){
  if(enabled){
    TIM2->CCR1 = 0x0000;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    DAC3->CR &= ~DAC_CR_EN1;
  }
  else{
    TIM2->CR1 |= TIM_CR1_CEN;
    DAC3->CR |= DAC_CR_CEN1;
  }

  enabled = ~enabled;
}