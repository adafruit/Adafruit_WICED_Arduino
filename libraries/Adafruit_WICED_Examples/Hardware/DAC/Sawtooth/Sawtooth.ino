#include <libmaple/dac.h>

void setup() 
{
  dac_enable_channel(DAC, 1); // Configures Pin A4 as DAC1
  dac_init(DAC, DAC_CH1);     // Start DAC1
}

void loop() 
{
  uint32_t i = 0;
  for ( i=0; i<4096; i++ )
  {
    dac_write_channel(DAC, DAC_CH1, i);
  }  
  for ( i=4095; i>0; i-- )
  {
    dac_write_channel(DAC, DAC_CH1, i);
  }  
}
