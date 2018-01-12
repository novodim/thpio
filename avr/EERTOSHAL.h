#ifndef EERTOSHAL_H
#define EERTOSHAL_H

#define STATUS_REG 			SREG
#define Interrupt_Flag		SREG_I
#define Disable_Interrupt	cli();
#define Enable_Interrupt	sei();

//RTOS Config
#define RTOS_ISR  			TIMER2_COMPA_vect
#define	TaskQueueSize		20
#define MainTimerQueueSize	15

#define Prescaler	64
#define TimerDivider            (F_CPU/Prescaler/1000)

extern void RunRTOS (void);

#endif
