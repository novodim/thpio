#include "main.h"
#include "hal.h"
#include "status.h"

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


void InitHw() {
        // set led pin as output
    sbi(LED_DDR,LED_PIN);
#if ( OUT_NUM_BUSES > 0 )
    sbi(OUT_B1_DDR,OUT_B1_PIN);
#endif


}

void Blink () {
    ibi(LED_OUT,LED_PIN);
}



