#include "Type.h" 
 
#ifndef DHT11_H    
#define DHT11_H
 
extern u8 rec_dat[9];
 
extern void DHT11_start();
extern u8 DHT11_rec_byte();
extern void DHT11_receive();
 
#endif 