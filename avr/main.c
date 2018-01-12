#include "main.h"
#include "hal.h"
#include "status.h"
#include "sensors.h"

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

volatile status_t s;


ISR(RTOS_ISR) {
TimerService();
}

void BlinkPeriodically(void) {
SetTimerTask(BlinkPeriodically,250);
Blink();
}

void SendStatusPeriodically(void) {
SetTimerTask(SendStatusPeriodically,3000);
SendStatus();
}

void ScanSensorsPeriodically(void) {
Blink();
SetTimerTask(ScanSensorsPeriodically,BUS_REFRESH_MS);
ScanSensors();
}

int main(void) {
char c;

USART_Init(MYUBRR);
InitRTOS();
RunRTOS();
InitHw();
InitStatus();

//BlinkPeriodically();
//SendStatusPeriodically();
//ScanSensorsPeriodically();
SetTask(BlinkPeriodically);
//SetTask(SendStatusPeriodically);
SetTask(ScanSensorsPeriodically);

while(1) {
    c=USART_ReceiveNB();
    switch(c) {
    case 'x':
        SendStatus();
        break;
    case 'a':
    cbi(OUT_B1_OUT,OUT_B1_PIN);
    cbi(s.out,0);
        break;
    case 'A':
    sbi(OUT_B1_OUT,OUT_B1_PIN);
    sbi(s.out,0);
        break;
    }
    TaskManager();
}
return 0;
}

