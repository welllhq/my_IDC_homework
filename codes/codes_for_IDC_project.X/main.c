#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif
#include"stdio.h"
#include<stdlib.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include"lcd.h"
#include"dht11.h"
#include "user.h"          /* User funct/params, such as InitApp */

// BEGIN CONFIG
#pragma config FOSC = XT // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//END CONFIG

#define _XTAL_FREQ  4000000
#define key RB0 //RB0 connect to the button
#define voltage_read_channel 0//AN0/RA0 connect to the voltage divider 
#define ldr_read_channel 1//AN1/RA1 connect to the voltage divider of LDR.
#define uint unsigned int
#define elif else if
/*************************************************************************************/
uint button_scan(); //Aim to scan whether the button has been pressed
uint analog_read(uint channel);//Aim to read the analog value once
uint voltage_read();//Aim to calculate the voltage value
uint light_level_check();//Aim to judge the level of light
void mode_welcome();//mdoe 0
void mode_voltage_read();//mode 1
void mode_light_level_check();//mode 2
void mode_dht11_read();//mode 3
/****************************************************************************************/
void main(void)
{
    uint mode_choose_flag = 0 ;            /***************************************/
                                           /*Aim to choose the mode when button is*/
                                           /*pressed.                             */
                                           /*MODE0 - welcome message              */
                                           /*MODE1 - voltage message              */
                                           /*MODE2 - level message of light       */
                                           /*MODE3 - message oftemperature and    */
                                           /*        humidity                     */
                                           /***************************************/
    InitApp();
    Lcd_Init(); //Initialize LCD
    __delay_ms(2000);
    RA2 = 0; //Turn off the buzzer
    while(1)
    {
        if(button_scan()) mode_choose_flag++;
        if(mode_choose_flag>3) mode_choose_flag = 1; //won't choose the MODE0  twice.
        switch(mode_choose_flag){
            case 0:
                RA2 = 0;//Turn off buzzer
                mode_welcome();
                break;
            case 1:
                RA2 = 0;//Turn off buzzer
                mode_voltage_read();
                break;
            case 2:
                RA2 = 0;//Turn off buzzer
                mode_light_level_check();
                break;
            case 3:
                mode_dht11_read();
                break;
        }
        __delay_ms(100);
    }

}

uint button_scan(){
    static uint key_up = 1; //button released flag
    if(key_up==1&&key == 1){
        //If not release the button,then key_up==0 and this part won't be executed.
        __delay_ms(10);//This line aim to avoid shake in real world,
                      //but it may cause some bugs when simulation,note it if necessary.
        key_up = 0;//The button has been pressed
        if(key==1)return 1;
    }
        else if(key==0)key_up = 1;//The button has been released
        return 0;
}
uint analog_read(uint channel){
    /*configure the registers*/
    ADFM = 1;
    ADCS2 = 1;
    VCFG0 = 0;
    VCFG1 = 0;
    ADCS0 = 0;
    ADCS1 = 0;
    ADCON0bits.CHS = channel;
    ADON =1 ;

    __delay_ms(1);
    GO_DONE = 1;
    while(ADCON0bits.GO_DONE);
    uint ad_value = (ADRESH << 8) + ADRESL;
    return ad_value;//return result

}
uint voltage_read(){
    uint voltage ;
    uint sum_voltage=0;
    for(int i=0;i<5;i++){
        voltage = analog_read(voltage_read_channel);
        __delay_ms(1);//wait for adc
        sum_voltage=sum_voltage + voltage;
    }
    return (sum_voltage/5);//Seeking average
}
uint light_level_check(){
    uint sum_light_level_value =0;
    uint ldr_value = analog_read(ldr_read_channel);
     for(int i=0;i<5;i++){
        uint ldr_value = analog_read(ldr_read_channel);
        sum_light_level_value=sum_light_level_value + ldr_value ;
    }
    return (sum_light_level_value/5);//Seeking average
}
void mode_welcome(){
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("WELCOME");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("PRESS THE BUTTON");
}
void mode_voltage_read(){
    unsigned char* value;
    float t1 = (((float)voltage_read())*(0.004887));//(5/1023)=0.004887
    int status;
    value = ftoa(t1,&status);//float to char
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("voltage(V) is:");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(value);
}
void mode_light_level_check(){
    uint t1 = light_level_check();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Light_LEVEL is:");
    Lcd_Set_Cursor(2,1);
    if(t1>816) Lcd_Write_String("DRAK");
    elif((t1<=816)&&(t1>204))Lcd_Write_String("Bright");
    elif(t1<=204)Lcd_Write_String("Dazzling");
}
void mode_dht11_read(){
    int temp,humi,dhumi,dtemp;
    unsigned char v_temp[4],v_humi[4];
    DHT11_init();
    if((DHT11_CheckResponse())==1){
        if((DHT11_ReadData(&humi,&dhumi,&temp,&dtemp))==1){
            Lcd_Clear();
            itoa(v_temp,temp,10);//Int to char
            itoa(v_humi,humi,10);//Int to char
            if(temp>35||temp<20||humi>75||humi<15)RA2 =1;
            else RA2 =0;
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("TEMP:");
            Lcd_Write_String(v_temp);
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("HUMI:");
            Lcd_Write_String(v_humi);
           }
       elif((DHT11_ReadData(&humi,&dhumi,&temp,&dtemp))== -1){
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("DHT11 ERROR:-1");//-1:dht11 working error,It seems that there
                                               //are some bugs of dht11 sensor on proteus.
        }
        elif((DHT11_ReadData(&humi,&dhumi,&temp,&dtemp))==2){
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("DHT11 ERROR:2");//2:data check error;
        }
        else{
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("DHT11 ERROR:0");//1:data read error
        }
    }
    else{
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("DHT11 ERROR:3");//3:response error
    }

}