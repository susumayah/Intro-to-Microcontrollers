/* 
 * File:   lab5_lcd.h
 * Author: sumay
 *
 * Created on November 19, 2024, 12:38 PM
 */
#ifndef LCD_HEADER
#define LCD_HEADER

#include <xc.h>
#include <stdint.h> // For standard integer types
#include <stdbool.h> // For boolean types
#include <string.h> // For string manipulation
#include <libpic30.h> // For __delay_ms function

#define _XTAL_FREQ 16000000 // Define the crystal frequency for delay functions
#define FCY (_XTAL_FREQ / 2)

#define LCD_I2C_ADDRESS 0x3C // Replace with correct LCD I2C address if different

// Function prototypes for the LCD library
void i2c_init(void);
void lcd_init(void);
void lcd_cmd(char command);
void lcd_setCursor(char x, char y);
void lcd_printChar(char myChar);
void lcd_printStr(const char *str);
void lcd_scrollMsg(const char *msg);
#ifdef __cplusplus
}
#endif

#endif /* LCD_HEADER */


