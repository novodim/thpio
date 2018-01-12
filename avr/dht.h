#ifndef DHT11_H_
#define DHT11_H_

#define DHT_TIMEOUT 200

extern void dht11_set_bus( volatile uint8_t*, volatile uint8_t* , volatile uint8_t* , volatile uint8_t );
int GetDhtValues (int *);
#endif
