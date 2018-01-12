#include "main.h"
#include "hal.h"
#include "status.h"

#include "ds.h"
#include "uart.h"
#include "dht.h"
#include "mpx.h"
#include "in.h"
#include "out.h"
#include "EERTOS.h"
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "avrlibtypes.h"

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include <stdint.h>

uint16_t mpx_scan(uint8_t adcx) {
ADCSRA |= _BV(ADEN);

#if ( MPX_VREF_SOURCE_VINT == 1 )
ADMUX |= (1<<REFS0)|(1<<REFS1);
#endif

#if ( MPX_VREF_SOURCE_VCC == 1 )
ADMUX |= (1<<REFS0);
#endif

#if ( MPX_VREF_SOURCE_VEXT == 1 )
#endif

 //set prescaller to 128 and enable ADC 
ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);    
//select ADC channel with safety mask
 ADMUX = (ADMUX & 0xF0) | (adcx & 0x0F);
 //single conversion mode
 ADCSRA |= (1<<ADSC);
 // wait until ADC conversion is complete
 while( ADCSRA & (1<<ADSC) );
 return ADC;
}
