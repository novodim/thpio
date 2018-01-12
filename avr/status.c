#include "main.h"
#include "hal.h"
#include "status.h"

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "EERTOS.h"
#include "uart.h"
#include "onewire.h"

extern volatile status_t s;


void InitStatus(void) {
    s.status_size=sizeof(s);
    s.ds_info=(DS_NUM_BUSES<<4)+(DS_BUS_MAX_SENS&0xf);
    s.dht_info=DHT_NUM_BUSES;
    s.mpx_info=MPX_NUM_BUSES;
    s.in_info=IN_NUM_BUSES;
    s.out_info=OUT_NUM_BUSES;
}

void SendStatus (void)
{
        USART_TransmitNBBinStr((char*)&s,sizeof(s));
}
