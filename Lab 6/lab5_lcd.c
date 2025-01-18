#include "lab5_lcd.h"
#include <stdint.h> // For standard integer types
#include <stdbool.h> // For boolean types
#include <string.h> // For string manipulation
#include <libpic30.h> // For __delay_ms function


// Clock frequency, FCY for delays and configure I2C
#define _XTAL_FREQ 16000000 // Define the crystal frequency for delay functions
#define FCY (_XTAL_FREQ / 2)
// wait for acknowledgment after sending data. _MI2C1IF is I2C interrupt flag
// when byte is transmitted
//TRSTAT transmission status bit, cleared when transmission complete
#define wait_for_ack() _MI2C1IF = 0; while (_MI2C1IF == 0 || I2C1STATbits.TRSTAT == 1);

//I2C intialization
void i2c_init(void) {
    I2C1BRG = 157; // Set baud rate to 100kHz 
    I2C1CONbits.I2CEN = 1; // Enable I2C module
    IFS1bits.MI2C1IF = 0; // Clear interrupt flag
}
// LCD initialization
void lcd_init(void) {
    // Reset RB6 to start with it turned off
    TRISBbits.TRISB6 = 0; // Set RB6 as output
     LATBbits.LATB6 = 0; // Set RB6 low
    _delay_ms(10); // Keep RB6 low for 10 ms
    LATBbits.LATB6 = 1; // Set RB6 high to turn it on
    _delay_ms(10); // Keep RB6 low for 10 ms
    lcd_cmd(0x3A); // Function set (RE=1)
    lcd_cmd(0x09); // Extended function set (4 line display)
    lcd_cmd(0x06); // Entry mode set (bottom view)
    lcd_cmd(0x1E); // Bias setting BS1=1
    lcd_cmd(0x39); // Function set (RE=0, IS=1)
    lcd_cmd(0x1B); // Internal OSC (BS0=1 -> Bias=1/6)
    lcd_cmd(0x6E); // Follower control (divider on and set value)
    lcd_cmd(0x56); // Power control (booster on and set contrast)
    lcd_cmd(0x7A); // Contrast set (DB3-DB0 = C3-C0)
    lcd_cmd(0x38); // Function set (RE=0, IS=0)
    lcd_cmd(0x0F); // Display ON, cursor on, blink on
    lcd_cmd(0x3A); // Function set (RE=1)
    lcd_cmd(0x09); // Extended function set
    lcd_cmd(0x1A); // Double-height setting
    lcd_cmd(0x3C); // Function set (RE=0)
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x56);

   
 

}
// sends I2C command to LCD
// Start condition SEN begins comm
// Address and command byte sent
// Stop condition PEN terminates communication

void lcd_cmd(char command) {
    LATBbits.LATB5 = 1;
    // Implement I2C write command
    I2C1CONbits.SEN = 1; // Initiate START condition
    while (I2C1CONbits.SEN); // Wait for START condition to complete

    I2C1TRN = (LCD_I2C_ADDRESS << 1) & 0xFE; // Send address with write bit
    wait_for_ack();
 
    I2C1TRN = 0x00; // Control byte
        wait_for_ack();

    I2C1TRN = command; // Send command
        wait_for_ack();
    I2C1CONbits.PEN = 1; // Initiate STOP condition
    while (I2C1CONbits.PEN); // Wait for STOP condition to finish
     LATBbits.LATB5 = 0;
}

void lcd_printChar(char myChar) {
    LATBbits.LATB5 = 1;
    // Implement I2C write command
    I2C1CONbits.SEN = 1; // Initiate START condition
    while (I2C1CONbits.SEN); // Wait for START condition to complete

    I2C1TRN = (LCD_I2C_ADDRESS << 1) & 0xFE; // Send address with write bit
    wait_for_ack();
 
    I2C1TRN = 0x40; // Control byte
    wait_for_ack();

    I2C1TRN = myChar; // Send command
    wait_for_ack();
    I2C1CONbits.PEN = 1; // Initiate STOP condition
    while (I2C1CONbits.PEN); // Wait for STOP condition to finish
    LATBbits.LATB5 = 0;
}

void lcd_printStr(const char *str) {
    while (*str) {
        lcd_printChar(*str++); // Print each character
    }
}

// Sets cursor position
void lcd_setCursor(char x, char y) {
    char address = 0x80 + (y * 0x20) + x; // Calculate address based on row and column
    lcd_cmd(address);
}
// Sends single characters
void lcd_scrollMsg(const char *msg, const char *staticMsg) {
    int len = strlen(msg);
    char buffer[17]; // Buffer for a single line of text
    memset(buffer, ' ', 16); // Initialize with spaces
    buffer[16] = '\0';       // Null terminate the buffer

    int scrollIndex = 0;

    while (1) { // Infinite loop for continuous scrolling
        // Build the buffer with the current scroll position
        for (int i = 0; i < 16; i++) {
            if (scrollIndex + i < len) {
                buffer[i] = msg[(scrollIndex + i) % len]; // Wrap around message when reaching the end
            } else {
                buffer[i] = ' '; // Fill with spaces after message ends
            }
        }

        // Update the first row (scrolling message)
        lcd_setCursor(0, 0); // Set cursor to the start of the first row
        lcd_printStr(buffer); // Print the buffer to the display

        // Update the second row (static message)
        lcd_setCursor(0, 1); // Set cursor to the start of the second row
        lcd_printStr(staticMsg); // Print the static message

        // Scroll the message
        scrollIndex = (scrollIndex + 1) % (len + 16); // Reset index after full scroll
        _delay_ms(50); // Adjust delay for scrolling speed
    }
}




void _delay_ms(unsigned int ms) {
    unsigned long cycles = (unsigned long)ms * (FCY / 1000);
    while (cycles > 0) {
        __asm__ volatile ("nop");
        cycles--;
    }
}

