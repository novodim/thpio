#ifndef status_h
#define status_h

#include "main.h"
#include "hal.h"
#include "status.h"

#include "EERTOS.h"
#include "onewire.h"
#include "ds.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include "avr/pgmspace.h"
#include <stdint.h>



typedef struct owd {
        unsigned char id[8];
} owd_t;

typedef struct status {
	int status_size;
	char ds_info;
	char ds_busError;
        char dht_info;
        char dht_busError;
        char mpx_info;
        char mpx_busError;
        char in_info;
        char out_info;
#if ( DS_NUM_BUSES > 0 )
       	owd_t ds_b1_ids[DS_BUS_MAX_SENS];
        int ds_b1_raws[DS_BUS_MAX_SENS];
#endif
#if ( DS_NUM_BUSES > 1 )
        owd_t ds_b2_ids[DS_BUS_MAX_SENS];
        int ds_b2_raws[DS_BUS_MAX_SENS];
#endif
#if ( DS_NUM_BUSES > 2 )
        owd_t ds_b3_ids[DS_BUS_MAX_SENS];
        int ds_b3_raws[DS_BUS_MAX_SENS];
#endif
#if ( DHT_NUM_BUSES > 0 )
        int dht11_b1_raw;
#endif
#if ( DHT_NUM_BUSES > 1 )
        int dht11_b2_raw;
#endif
#if ( MPX_NUM_BUSES > 0 )
        uint16_t mpx_b1;
#endif
#if ( MPX_NUM_BUSES > 1 )
        uint16_t mpx_b2;
#endif
        uint8_t in;
        uint8_t out;
} status_t;

void InitStatus(void);
void SendStatus (void);

#endif
