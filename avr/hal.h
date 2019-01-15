#ifndef hal_h
#define hal_h

// every this period refresh sensors values for each bus in turn
#define BUS_REFRESH_MS 60000

// number of ds sensors used
#define DS_NUM_BUSES 0
#if ( DS_NUM_BUSES > 3 )
#error DS_NUM_BUSES cannot be greater than 3
#endif
#define DS_BUS_MAX_SENS 2

#define DS_B1_IN   	PIND
#define DS_B1_PIN  	PD2
#define DS_B1_OUT  	PORTD
#define DS_B1_DDR  	DDRD

#define DS_B2_IN    PIND
#define DS_B2_PIN   PD3
#define DS_B2_OUT   PORTD
#define DS_B2_DDR   DDRD

#define DS_B3_IN    PIND
#define DS_B3_PIN   PD4
#define DS_B3_OUT   PORTD
#define DS_B3_DDR   DDRD

// number of dht11 buses used
#define DHT_NUM_BUSES 0
#define DHT_B1_IN    PIND
#define DHT_B1_PIN   PD5
#define DHT_B1_OUT   PORTD
#define DHT_B1_DDR   DDRD

#define DHT_B2_IN    PIND
#define DHT_B2_PIN   PD6
#define DHT_B2_OUT   PORTD
#define DHT_B2_DDR   DDRD

// configuration of ADC converter for all analog inputs
// select the only one from 3 options
#define MPX_VREF_SOURCE_VCC  0
#define MPX_VREF_SOURCE_VEXT 1
#define MPX_VREF_SOURCE_VINT 0

//number of mpx sensors used
#define MPX_NUM_BUSES 2
#define MPX_B1_ADC_PORT 0
#define MPX_B2_ADC_PORT 1

//number of digital in pins used
#define IN_NUM_BUSES 0
#if ( IN_NUM_BUSES > 8 )
#error IN_NUM_BUSES cannot be greater than 8
#endif

#define IN_B1_IN    PIND
#define IN_B1_PIN   PD5
#define IN_B1_OUT   PORTD
#define IN_B1_DDR   DDRD

#define IN_B2_IN    PIND
#define IN_B2_PIN   PD5
#define IN_B2_OUT   PORTD
#define IN_B2_DDR   DDRD

#define IN_B3_IN    PIND
#define IN_B3_PIN   PD5
#define IN_B3_OUT   PORTD
#define IN_B3_DDR   DDRD

#define IN_B4_IN    PIND
#define IN_B4_PIN   PD5
#define IN_B4_OUT   PORTD
#define IN_B4_DDR   DDRD

//number of digital out pins used
#define OUT_NUM_BUSES 1
#if ( OUT_NUM_BUSES > 8 )
#error OUT_NUM_BUSES cannot be greater than 8
#endif

#define OUT_B1_IN    PIND
#define OUT_B1_PIN   PD2
#define OUT_B1_OUT   PORTD
#define OUT_B1_DDR   DDRD

//general arduino led
#define LED_IN       PINB
#define LED_PIN      PB5
#define LED_OUT      PORTB
#define LED_DDR      DDRB

void InitHw(void);
void Blink (void);

#endif
