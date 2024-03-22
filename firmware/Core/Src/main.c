#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "opamp.h"
#include "tim.h"
#include "ucpd.h"
#include "usb.h"
#include "gpio.h"
#include "ctype.h"

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC3_Init();
  MX_OPAMP1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_UCPD1_Init();
  MX_USB_PCD_Init();

  tusb_init();
  tuc_init(0, TUSB_TYPEC_PORT_SNK);

  while (1)
  {
    tud_task();
    tuc_task();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
