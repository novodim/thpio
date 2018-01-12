#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"
#include "hal.h"
#include "status.h"

#include "uart.h"
#include "dht11.h"


volatile uint8_t* DHT_IN;
volatile uint8_t* DHT_OUT;
volatile uint8_t* DHT_DDR;                                                                                                                                                          
volatile uint8_t DHT_PIN;                                                                                                                                                           
                                                                                                                                                                                    
                                                                                                                                                                                    
extern void dht11_set_bus( volatile uint8_t* in, volatile uint8_t* out, volatile uint8_t* ddr, volatile uint8_t pin ) {                                                             
    DHT_DDR=ddr;                                                                                                                                                                    
    DHT_OUT=out;
    DHT_IN=in;
    DHT_PIN=pin;
}

int GetDhtValues (int * v) {
    uint8_t bits[5];
    uint8_t i,j = 0;
    memset(bits, 0, sizeof(bits));
    cli();
    //reset port
    *DHT_DDR |= (1<<DHT_PIN); //output
    *DHT_OUT |= (1<<DHT_PIN); //high
    _delay_ms(100);
    //send request
    *DHT_OUT &= ~(1<<DHT_PIN); //low
    _delay_ms(18);
    *DHT_OUT |= (1<<DHT_PIN); //high
    *DHT_DDR &= ~(1<<DHT_PIN); //input
    _delay_us(40);
    //check start condition 1
    if((*DHT_IN & (1<<DHT_PIN))) {
        sei();
        return -1;
        }
    _delay_us(80);
    //check start condition 2
    if(!(*DHT_IN & (1<<DHT_PIN))) {
        sei();
        return -1;
        }
    _delay_us(80);

    //read the data
    uint16_t timeoutcounter = 0;
    for (j=0; j<5; j++) { //read 5 byte
        uint8_t result=0;
        for(i=0; i<8; i++) {//read every bit
            timeoutcounter = 0;
            while(!(*DHT_IN & (1<<DHT_PIN))) { //wait for an high input (non blocking)
                timeoutcounter++;
                if(timeoutcounter > DHT_TIMEOUT) {
                    sei();
                    return -1; //timeout
                    }
                }
           _delay_us(30);
           if(*DHT_IN & (1<<DHT_PIN)) //if input is high after 30 us, get result
               result |= (1<<(7-i));
           timeoutcounter = 0;
           while(*DHT_IN & (1<<DHT_PIN)) { //wait until input get low (non blocking)
               timeoutcounter++;
               if(timeoutcounter > DHT_TIMEOUT) {
                    sei();
                    return -1; //timeout
                    }
               }
       }
       bits[j] = result;
   }

    //reset port
    *DHT_DDR |= (1<<DHT_PIN); //output
    *DHT_OUT |= (1<<DHT_PIN); //low
    _delay_ms(100);
    sei();
    if((bits[0]+bits[2])==bits[4]) {
        *v=(bits[0]<<8)+bits[2];
        } else return 1;
    return 0;
}

