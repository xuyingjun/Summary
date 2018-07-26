

#ifndef RTD_H
#define RTD_H
void RTD_init(void);
void RTD_onechannel(int channel,float* temperature);
void RTD_allchannels(float* temperature);
#endif
