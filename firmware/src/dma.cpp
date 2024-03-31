#include "dma.h"

void init_dma(void)
{
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  // HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  // NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  // NVIC_EnableIRQ(DMA1_Channel2_IRQn);

  // NVIC_SetPriority(DMA1_Channel3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  // NVIC_EnableIRQ(DMA1_Channel3_IRQn);

  // HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

  // HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}