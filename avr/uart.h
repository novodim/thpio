#ifndef uart_h
#define uart_h

// ring buffer length
#define rb_len 32

// FOSC Must be defiend externally, if not - uncomment
#define FOSC 16000000

// with 1M RC oscillator 1200 baud is reliable
//#define BAUD 38400
//#define BAUD 57600
#define BAUD 9600
//#define BAUD 115200

#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int );
void USART_TransmitNB( unsigned char );
void USART_TransmitNBStr(const char * );
void USART_TransmitNBBinStr(const char *,int);
void USART_Transmit(unsigned char );
void USART_TransmitStr(const char *);
unsigned char USART_Receive( void );
unsigned char USART_ReceiveNB( void );

#endif
