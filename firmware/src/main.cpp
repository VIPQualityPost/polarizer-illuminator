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
#include "PeripheralPins.h"
#include "USBPowerDelivery.h"

#define USBD_MANUFACTURER_STRING "matei repair lab"
#define USBD_PRODUCT_STRING_FS "illuminator"

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac3;
extern OPAMP_HandleTypeDef hopamp1;

RTTStream rtt;

bool IlluminatorEnabled = true;
bool togglePolarizer = true;

volatile uint16_t adc_data[2] = {0};

void userButton(void){
  if(IlluminatorEnabled){
    PowerSink.requestPower(5000, 2000);
    DAC3->CR &= ~DAC_CR_CEN1;
  }
  else{
    PowerSink.requestPower(15000, 2000);
    DAC3->CR |= DAC_CR_CEN1;
  }

  IlluminatorEnabled = !IlluminatorEnabled;
}

void updateControls(void){
  if(togglePolarizer)
    DAC3->DHR12R1 = 0x000;
  else
    DAC3->DHR12R1 = analogRead(LEFT_POT) << 2;

  TIM2->CCR1 = analogRead(RIGHT_POT) << 6;
  togglePolarizer = !togglePolarizer;
}

void handlePDEvent(PDSinkEventType evt){
  if(evt == PDSinkEventType::sourceCapabilitiesChanged){
    // Search for 15V PDO
    for(int i = 0; i < PowerSink.numSourceCapabilities; i+= 1){
      if(PowerSink.sourceCapabilities[i].minVoltage <= 15000 && 
        PowerSink.sourceCapabilities[i].maxVoltage >=15000){
          PowerSink.requestPower(15000);
          return;
      }
    }
    // Try for 12V if no 15V PDO
    for(int i = 0; i < PowerSink.numSourceCapabilities; i += 1){
      if(PowerSink.sourceCapabilities[i].minVoltage <= 12000 &&
        PowerSink.sourceCapabilities[i].maxVoltage >=12000){
          PowerSink.requestPower(12000);
          return;
      }
    }

  PowerSink.requestPower(9000);
  }

}

void setup() {
  pinMode(USER_BUTTON, INPUT);
  pinMode(LEFT_POT, INPUT_ANALOG);
  pinMode(RIGHT_POT, INPUT_ANALOG);
  pinMode(LED_PWM, OUTPUT);
  pinMode(LCD_PWM, OUTPUT);
  pinMode(LCD_OPA, OUTPUT);

  PowerSink.start();
  PowerSink.requestPower(9000);
  PowerSink.requestPower(12000);
  PowerSink.requestPower(15000);

  // digitalWrite(LED_PWM, HIGH);

  // __HAL_RCC_DMAMUX1_CLK_ENABLE();
  // __HAL_RCC_DMA1_CLK_ENABLE();

  // gpio_config_adc(&hadc1);
  // init_adc();
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 2);

  __HAL_RCC_DAC3_CLK_ENABLE();
  init_dac();

  gpio_config_opa(&hopamp1);
  init_opa();
  
  HAL_OPAMP_Start(&hopamp1);
  HAL_DAC_Start(&hdac3, DAC_CHANNEL_1);
  
  TIM_TypeDef* IlluminatorTimer = TIM2;
  uint32_t IlluminatorChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(LED_PWM), PinMap_PWM));
  HardwareTimer* Illuminator = new HardwareTimer(IlluminatorTimer);
  Illuminator->setMode(IlluminatorChannel, TIMER_OUTPUT_COMPARE_PWM1, LED_PWM);
  Illuminator->setOverflow(5000, HERTZ_FORMAT);

  TIM_TypeDef *PolarizerTimer = TIM1;
  HardwareTimer* Polarizer = new HardwareTimer(PolarizerTimer);
  Polarizer->attachInterrupt(updateControls);
  Polarizer->setOverflow(1000, HERTZ_FORMAT);

  Illuminator->resume();
  Polarizer->resume();

  attachInterrupt(PB8, userButton, RISING);
}

void loop(){ 
}

