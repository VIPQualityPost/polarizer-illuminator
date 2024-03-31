#include <Arduino.h>
#include "RTTStream.h"
#include "dac.h"

DAC_HandleTypeDef hdac3;

extern RTTStream rtt;

void init_dac(void)
{

  DAC_ChannelConfTypeDef sConfig = {0};

  hdac3.Instance = DAC3;
  
  if(HAL_DAC_Init(&hdac3) != HAL_OK){
    rtt.println("Issue initializing DAC.");
  }

  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  
  if(HAL_DAC_ConfigChannel(&hdac3, &sConfig, DAC_CHANNEL_1) != HAL_OK){
    rtt.println("Issue configuring DAC output channel.");
  }

  HAL_DACEx_SelfCalibrate(&hdac3, &sConfig, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac3, DAC_CHANNEL_1);

}

// void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
// {
//   if(dacHandle->Instance==DAC3)
//   {
//     __HAL_RCC_DAC3_CLK_ENABLE();
//   }
// }
