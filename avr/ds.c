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
#include <string.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include "avr/pgmspace.h"



extern volatile status_t s;


void ScanBus (char busid) {
    uint8_t diff;
    int i,h,l,ds_n=0;
    owd_t ds_ids[DS_BUS_MAX_SENS];
    int ds_raws[DS_BUS_MAX_SENS];
    memset (ds_raws,0,sizeof(ds_raws)); 	
    cli();
    switch(busid) {
#if ( DS_NUM_BUSES > 0 )
        case 0:
            ow_set_bus( &DS_B1_IN, &DS_B1_OUT, &DS_B1_DDR, DS_B1_PIN );
            break;
#endif
#if ( DS_NUM_BUSES > 1 )
        case 1:
            ow_set_bus( &DS_B2_IN, &DS_B2_OUT, &DS_B2_DDR, DS_B2_PIN );
            break;
#endif
#if ( DS_NUM_BUSES > 2 )
        case 2:
            ow_set_bus( &DS_B3_IN, &DS_B3_OUT, &DS_B3_DDR, DS_B3_PIN );
            break;
#endif
    }
    ow_reset();
    cbi(s.ds_busError,busid);
    for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE; ) {
        diff = ow_rom_search( diff, &ds_ids[ds_n].id[0] );
        if( diff == OW_PRESENCE_ERR ) {
            sbi(s.ds_busError,busid);
            sei();
            return;
            }
        if( diff == OW_DATA_ERR ) {
            sbi(s.ds_busError,busid);
            sei();
            return;
            }
        ds_n++;
        }
        ow_reset();

        ow_command(OW_SKIP_ROM,0);
        ow_command(0x44,0);
        ow_parasite_enable();
        _delay_ms(1000); 
        ow_parasite_disable();
        _delay_ms(300);
        if((s.ds_busError& (1<<busid)) == 0) {
            for(i=0;i<ds_n;i++) {
                ow_command(OW_READ_SP,&ds_ids[i].id[0]);  
                h=ow_byte_rd();
                l=ow_byte_rd();
                ds_raws[i]=h+(l<<8);
                ow_reset();
                }
        ow_reset();
        }
    sei();
    switch(busid) {
#if ( DS_NUM_BUSES > 0 )
        case 0:
	for (i=0;i<ds_n;i++) {for(h=0;h<8;h++)s.ds_b1_ids[i].id[h]=ds_ids[i].id[h];s.ds_b1_raws[i]=ds_raws[i];}
            break;
#endif
#if ( DS_NUM_BUSES > 1 )
        case 1:
	for (i=0;i<ds_n;i++) {for(h=0;h<8;h++)s.ds_b2_ids[i].id[h]=ds_ids[i].id[h];s.ds_b2_raws[i]=ds_raws[i];}
            break;
#endif
#if ( DS_NUM_BUSES > 2 )
        case 2:
	for (i=0;i<ds_n;i++) {for(h=0;h<8;h++)s.ds_b3_ids[i].id[h]=ds_ids[i].id[h];s.ds_b3_raws[i]=ds_raws[i];}
            break;
#endif
    }

}

