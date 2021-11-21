#ifndef __KNX_STACK__
#define __KNX_STACK__

void knx_init(void);

void knx_loop(void);

void knx_rx_interrupt(void);

void knx_tim_counter_elapsed(int channel);

#endif
