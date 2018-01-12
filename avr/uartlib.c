#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
int main () {
unsigned char c;
uart_init(9600);
sei();
uart_putc('h');
while(1) {
//c=uart_getc();
uart_putc('s');
UDR='a';
_delay_ms(100);
}
}
