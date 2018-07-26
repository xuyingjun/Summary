#ifndef _stm32ads1115_1_H_
#define _stm32ads1115_1_H_

#define MODE_CURRENT 1
#define MODE_VOLTAGE 0

void stm32ads1115_1_init(void);
void read_ADS1115_1(void);
float ReadAdOfChannel(unsigned char  Channel,unsigned char CurrentFlag);

#endif
