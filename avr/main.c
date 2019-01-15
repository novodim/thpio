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
    cbi(OUT_B1_OUT,0);
    cbi(s.out,0);
        break;
    case 'A':
    sbi(OUT_B1_OUT,0);
    sbi(s.out,0);
        break;

    case 'b':
    cbi(OUT_B1_OUT,OUT_B1_PIN);
    cbi(s.out,1);
        break;
    case 'B':
    sbi(OUT_B1_OUT,OUT_B1_PIN);
    sbi(s.out,1);
        break;

    case 'c':
    cbi(OUT_B1_OUT,2);
    cbi(s.out,2);
        break;
    case 'C':
    sbi(OUT_B1_OUT,2);
    sbi(s.out,2);
        break;

    case 'd':
    cbi(OUT_B1_OUT,3);
    cbi(s.out,3);
        break;
    case 'D':
    sbi(OUT_B1_OUT,3);
    sbi(s.out,3);
        break;

    case 'e':
    cbi(OUT_B1_OUT,4);
    cbi(s.out,4);
        break;
    case 'E':
    sbi(OUT_B1_OUT,4);
    sbi(s.out,4);
        break;

    case 'f':
    cbi(OUT_B1_OUT,5);
    cbi(s.out,5);
        break;
    case 'F':
    sbi(OUT_B1_OUT,5);
    sbi(s.out,5);
        break;

    case 'g':
    cbi(OUT_B1_OUT,6);
    cbi(s.out,6);
        break;
    case 'G':
    sbi(OUT_B1_OUT,6);
    sbi(s.out,6);
        break;

    case 'h':
    cbi(OUT_B1_OUT,7);
    cbi(s.out,7);
        break;
    case 'H':
    sbi(OUT_B1_OUT,7);
    sbi(s.out,7);
        break;

    }
    TaskManager();
}
return 0;
}

