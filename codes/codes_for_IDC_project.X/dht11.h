/* 
 * File:   dht11.h
 * Author: Gabriela31 from:
 *         https://github.com/Gabriela31/DHT11-Ultrasonido-y-Serial-con-PIC18F4550
 * Modified by Wells(Yuchen Lu) to adapt the PIC16 and incress the portability
 * 
 */

#ifndef DHT11_H
#define	DHT11_H



void DHT11_init(void);
int DHT11_CheckResponse(void);
int DHT11_ReadData(int *hum,int *dhum,int *temp, int *dtemp);




#ifdef	__cplusplus
}
#endif

#endif	/* DHT11_H */

