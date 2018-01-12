#include <EERTOSHAL.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include "avr/pgmspace.h"
#include <avr/wdt.h>

#define LO(x) ((x)& 0xFF)

//RTOS Запуск системного таймера
inline void RunRTOS (void)
{
TCCR2A = 1<<WGM21;
TCCR2B = 4<<CS20;
TCNT2 = 0;
OCR2A  = LO(TimerDivider);
TIMSK2 = 0<<TOIE2|1<<OCIE2A|0<<OCIE2B;

/*#define TIMSK2 _SFR_MEM8(0x70)
#define TOIE2 0
#define OCIE2A 1
#define OCIE2B 2


#define TCCR2A _SFR_MEM8(0xB0)
#define WGM20 0
#define WGM21 1
#define COM2B0 4
#define COM2B1 5
#define COM2A0 6
#define COM2A1 7

#define TCCR2B _SFR_MEM8(0xB1)
#define CS20 0
#define CS21 1
#define CS22 2
#define WGM22 3
#define FOC2B 6
#define FOC2A 7

*/
sei();
}
