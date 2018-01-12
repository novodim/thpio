#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

// uart functions and USART_RXC_vect and USART_UDRE_vect for atmega8 handlers to work with uart

volatile unsigned char rb_rx_begin=0,rb_rx_end=0;
volatile unsigned char rb_tx_begin=0,rb_tx_end=0;
volatile unsigned char rb_rx[rb_len];
volatile unsigned char rb_tx[rb_len];

void USART_Init( unsigned int ubrr)
{
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
//UCSR0C = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_TransmitNB( unsigned char data )
{
	while(rb_tx_end+1==rb_tx_begin);
	while(rb_tx_begin==0&&rb_tx_end+1==rb_len);
	rb_tx[rb_tx_end]=data;
	rb_tx_end++;
	if(rb_tx_end==rb_len)rb_tx_end=0;
	UCSR0B |=_BV(UDRIE0);
}

void USART_TransmitNBStr(const char *s )
{
    while (*s) 
        USART_TransmitNB(*s++);

}

void USART_TransmitNBBinStr(const char *s,int len )
{
    for (int i=0;i<len;i++)
        USART_TransmitNB(*(s+i));

}


void USART_Transmit(unsigned char data )
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = data;
}

void USART_TransmitStr(const char *s )
{
    while (*s) 
      USART_Transmit(*s++);

}

unsigned char USART_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

unsigned char USART_ReceiveNB( void )
{
	unsigned char c;
	if(rb_rx_end==rb_rx_begin) return 0;
	c=rb_rx[rb_rx_begin];
	rb_rx_begin++;
	if(rb_rx_begin==rb_len)rb_rx_begin=0;
	return c;
}

ISR(USART_RX_vect)
{
	rb_rx[rb_rx_end]=UDR0;
	rb_rx_end++;
	if(rb_rx_end==rb_len)rb_rx_end=0;
}

ISR(USART_UDRE_vect)
{
	unsigned char c;
	if(rb_tx_end!=rb_tx_begin) {
	c=rb_tx[rb_tx_begin];
	rb_tx_begin++;
	if(rb_tx_begin==rb_len)rb_tx_begin=0;
	UDR0=c;
	} else {
	UCSR0B &= ~_BV(UDRIE0);
	}
}

