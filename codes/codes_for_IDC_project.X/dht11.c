#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "dht11.h"

#define _XTAL_FREQ 4000000
#define DHT11_Data_Pin_Direction TRISBbits.TRISB3//RB3 set as data pin
#define DHT11_Data_Pin PORTBbits.RB3//
#define uint unsigned int

void DHT11_init(void)
{
    DHT11_Data_Pin_Direction = 0;
    DHT11_Data_Pin = 0;
    __delay_ms(20);
    DHT11_Data_Pin_Direction = 1;
    __delay_us(13);
}

int DHT11_CheckResponse(void){
    uint cont = 0;
    while(DHT11_Data_Pin ==1){
        cont++;
        if (cont == 500){
            return 0;
        }
    }
    cont = 0;
    while(DHT11_Data_Pin == 0 ){
       cont++;
        if (cont == 500){
            return 0;
        }
    }
    cont = 0;
    while(DHT11_Data_Pin == 1){
            cont++;
        if (cont == 500){
            return 0;
        }
    }

    return 1; //response successful
}

int DHT11_ReadData(int *hum,int *dhum,int *temp, int *dtemp) {
    int bits[5];
    int data,cont_read;

    for (int i=0; i < 5; i++){
        data = 0;
        for(int j=0;j<8;j++)
        {
            cont_read = 0;
            while(DHT11_Data_Pin == 0){
                cont_read ++;
                if (cont_read == 500){
                    return 0;
                    }
                }
            __delay_us(30);
            if(DHT11_Data_Pin == 1) {
                data = ((data<<1)+1);
            }
             else{
                data = (data<<1);
                }
             cont_read = 0;
            while(DHT11_Data_Pin == 1){
                cont_read++;
                if (cont_read == 5000){
                    return -1;
                }
             }
            }
        bits[i] = data;
        }
    if ((bits[0] + bits[1] + bits[2] + bits[3]) == bits[4])
	{
        *hum = bits[0];
        *dhum = bits[1];
        *temp = bits[2];
        *dtemp = bits[3];
        return 1;
        }
    else return 2;
}
