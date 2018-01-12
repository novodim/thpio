#include "main.h"
#include "hal.h"
#include "status.h"

#include "EERTOS.h"
#include "onewire.h"
#include "ds.h"
#include "dht11.h"
#include "mpx.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include "avr/pgmspace.h"



extern volatile status_t s;


void ScanSensors() {
#if ( DS_NUM_BUSES > 0 )
    ScanBus(0);
#endif
#if ( DS_NUM_BUSES > 1 )
    ScanBus(1);
#endif
#if ( DS_NUM_BUSES > 2 )
    ScanBus(2);
#endif

#if ( DHT_NUM_BUSES > 0 )
	dht11_set_bus(&DHT_B1_IN, &DHT_B1_OUT, &DHT_B1_DDR, DHT_B1_PIN); 
	if(GetDhtValues((int*)&s.dht11_b1_raw)==-1)sbi(s.dht_busError,0);
#endif
#if ( DHT_NUM_BUSES > 1 )
        dht11_set_bus(&DHT_B2_IN, &DHT_B2_OUT, &DHT_B2_DDR, DHT_B2_PIN);
	if(GetDhtValues((int*)&s.dht11_b2_raw)==-1)sbi(s.dht_busError,1);
#endif
#if ( MPX_NUM_BUSES > 0 )
	s.mpx_b1=mpx_scan(MPX_B1_ADC_PORT);
#endif
#if ( MPX_NUM_BUSES > 1 )
    s.mpx_b2=mpx_scan(MPX_B2_ADC_PORT);
#endif
#if ( IN_NUM_BUSES > 0 )
	for (int i=0;i<IN_NUM_BUSES;i++) {
    switch(i) {
        case 0:
			if(IN_B1_IN&(1<<IN_B1_PIN))sbi(s.in,i); else cbi(s.in,i);
            break;
#if ( IN_NUM_BUSES > 1 )
        case 1:
			if(IN_B2_IN&(1<<IN_B2_PIN))sbi(s.in,i); else cbi(s.in,i);
            break;
#endif
#if ( IN_NUM_BUSES > 2 )
        case 2:
			if(IN_B3_IN&(1<<IN_B3_PIN))sbi(s.in,i); else cbi(s.in,i);
            break;
#endif
#if ( IN_NUM_BUSES > 3 )
        case 3:
            if(IN_B3_IN&(1<<IN_B4_PIN))sbi(s.in,i); else cbi(s.in,i);
            break;
#endif
    }
	}
#endif
}

