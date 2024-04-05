#include <Arduino.h>
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "opa.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"
#include "stm32g4xx_hal_dac.h"
#include "stm32g4xx_hal_dma.h"
#include "stm32g4xx_hal_opamp.h"

#include "RTTStream.h"
#include "USBPowerDelivery.h"

#define USBD_MANUFACTURER_STRING "matei repair lab"
#define USBD_PRODUCT_STRING_FS "illuminator"

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac3;
extern OPAMP_HandleTypeDef hopamp1;

RTTStream rtt;

void userButton();

bool IlluminatorEnabled = true;
volatile uint16_t adc_data[2] = {0};

void setup() {
  pinMode(PB8, INPUT);
  // pinMode(PA0, INPUT_ANALOG);
  // pinMode(PA1, INPUT_ANALOG);
  pinMode(PA5, OUTPUT);
  pinMode(PA8, OUTPUT);

  attachInterrupt(PB8, userButton, RISING);

  PowerSink.start();
  PowerSink.requestPower(15000);

  init_dma();

  gpio_config_adc(&hadc1);
  init_adc();
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 2);

  __HAL_RCC_DAC3_CLK_ENABLE();
  init_dac();

  gpio_config_opa(&hopamp1);
  init_opa();
  
  HAL_OPAMP_Start(&hopamp1);
  HAL_DAC_Start(&hdac3, DAC_CHANNEL_1);
  
  TIM_TypeDef* IlluminatorTimer = (TIM_TypeDef*)pinmap_peripheral(digitalPinToPinName(PA5), PinMap_PWM);
  uint32_t IlluminatorChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA5), PinMap_PWM));
  HardwareTimer* Illuminator = new HardwareTimer(IlluminatorTimer);
  Illuminator->setMode(IlluminatorChannel, TIMER_OUTPUT_COMPARE_PWM1, PA5);
  Illuminator->setOverflow(2000, HERTZ_FORMAT);
  Illuminator->setCaptureCompare(IlluminatorChannel, 90, HERTZ_COMPARE_FORMAT);

  // TIM2->CR1 |= 0b1 << TIM_CR1_URS_Pos;
  // TIM2->CR2 |= 0b010 << TIM_CR2_MMS_Pos;

  TIM_TypeDef *PolarizerTimer = (TIM_TypeDef*)pinmap_peripheral(digitalPinToPinName(PA8), PinMap_PWM);
  uint32_t PolarizerChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA8), PinMap_PWM));
  HardwareTimer* Polarizer = new HardwareTimer(PolarizerTimer);
  Polarizer->setMode(PolarizerChannel, TIMER_OUTPUT_COMPARE_PWM1, PA8);
  Polarizer->setOverflow(1000, HERTZ_FORMAT);
  Polarizer->setCaptureCompare(PolarizerChannel, 50, PERCENT_COMPARE_FORMAT);

  Illuminator->resume();
  Polarizer->resume();
}

void loop(){ 
  // DAC3->DHR12R1 = analogRead(PA0) << 2;
  // TIM2->CCR1 = analogRead(PA1) << 6;

  DAC3->DHR12R1 = adc_data[0] << 2;
  TIM2->CCR1 = adc_data[1] << 6;

  PowerSink.poll();
}

void userButton(void){
  if(IlluminatorEnabled){
    // Reduce losses from LDO
    PowerSink.requestPower(5000, 2000);
    IlluminatorEnabled = false;
    // Turn off peripherals, saves about 100mW idle
    TIM1->CR1 &= ~TIM_CR1_CEN;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    DAC3->CR &= ~DAC_CR_CEN1;
  }
  else{
    PowerSink.requestPower(15000, 2000);
    IlluminatorEnabled = true;
    // Turn on peripherals
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM2->CR1 |= TIM_CR1_CEN;
    DAC3->CR |= DAC_CR_CEN1;
  }
}
