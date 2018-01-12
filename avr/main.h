#ifndef main_h
#define main_h

#ifndef ibi
	#define ibi(reg,bit) reg ^= (1<<bit)
#endif
#ifndef cbi
    #define cbi(reg,bit)    reg &= ~(BV(bit))
#endif
#ifndef sbi
    #define sbi(reg,bit)    reg |= (BV(bit))
#endif


#endif
