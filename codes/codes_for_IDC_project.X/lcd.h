/* 
 * File:   lcd.h
 * Author: The third party library
 *
 * Created on 2020?4?15?, ??3:14
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus

#endif

//LCD Functions Developed by electroSome
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

