#include "dma.h"

void init_dma(void)
{
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  // HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

// extern "C" {
// void DMA1_Channel5_IRQHandler(void)
// {
//   HAL_DMA_IRQHandler(&hdma_adc1);
// }
// }