/*
 * File:   elmi0034_final_main.c
 * Author: sumay
 *
 * Created on November 19, 2024, 12:07 PM
 */
#pragma config ICS = PGx1 // Emulator EMUC1/EMUD1 pins shared with PGC1/PGD1
#pragma config FWDTEN = OFF // Disable Watchdog Timer
#pragma config GWRP = OFF // General Code Segment Write Protect
#pragma config GCP = OFF // General Code Segment Code Protect
#pragma config JTAGEN = OFF // Disable JTAG Port
#pragma config I2C1SEL = PRI // Default SCL1/SDA1 pins
#pragma config IOL1WAY = OFF // IOLOCK can be changed
#pragma config OSCIOFNC = ON // CLKO functions as I/O pin
#pragma config FCKSM = CSECME // Clock switching enabled, Fail-Safe Clock Monitor
#pragma config FNOSC = FRCPLL // Fast RC Oscillator with PLL
#include "circBuff.h"
#include "final_lcd.h"
#include "sensor.h"      // Sensor functions (light and moisture)
#include <stdio.h>
#include <libpic30.h> // For __delay_ms function
#define LIGHT_THRESHOLD 2223
#define V_THRESHOLD 2.5  // Voltage threshold for moisture detection
#define ADC_RESOLUTION 1024
#define _XTAL_FREQ 16000000 // Define the crystal frequency for delay functions
#define FCY (_XTAL_FREQ / 2)
#define wait_for_ack() _MI2C1IF = 0; while (_MI2C1IF == 0 || I2C1STATbits.TRSTAT == 1);
#define VREF 3.3 // Reference voltage (adjust as needed for your system)
#define BUFFER_SIZE 1024
#define SAMPLES 128
#define PERIOD 5  // Time in milliseconds per frame



static int buffer[BUFFER_SIZE];
static int front = SAMPLES;
static int back = 0;
static long sum = 0;
static int count = 0;

void delay_ms(int ms) {
    unsigned long cycles = (unsigned long) ms * (FCY / 1000);
    while (cycles > 0) {
        __asm__ volatile ("nop");
        cycles--;
    }
}
void i2c_init(void) {
    I2C1BRG = 157; // Set baud rate generator for 100kHz with Fcy = 16MHz
    I2C1CONbits.I2CEN = 1; // Enable I2C module
    IFS1bits.MI2C1IF = 0; // Clear interrupt flag
}

void i2c_start(void) {
    I2C1CONbits.SEN = 1;  // Initiate Start condition
    while (I2C1CONbits.SEN);
}

void i2c_stop(void) {
    I2C1CONbits.PEN = 1;  // Initiate Stop condition
    while (I2C1CONbits.PEN);
}

void i2c_write(unsigned char data) {
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT);  // Wait for transmission to complete
    if (I2C1STATbits.ACKSTAT) {
        // Handle NACK (optional)
    }
}

unsigned char i2c_read(unsigned char ack) {
    I2C1CONbits.RCEN = 1;  // Enable receive mode
    while (!I2C1STATbits.RBF);  // Wait for data
    unsigned char data = I2C1RCV;  // Read received byte
    I2C1CONbits.ACKDT = ack;  // Set acknowledgment bit (0 = ACK, 1 = NACK)
    I2C1CONbits.ACKEN = 1;  // Send ACK/NACK
    while (I2C1CONbits.ACKEN);
    return data;
}
void initTSL2561(void) {
    i2c_start();
    i2c_write((TSL2561_ADDR << 1) | 0);
    i2c_write(0x80 | 0x00);  // Control register
    i2c_write(0x03);         // Power ON
    i2c_stop();
}

float readLightLevel(void) {
    unsigned char low, high;

    i2c_start();
    i2c_write((TSL2561_ADDR << 1) | 0);
    i2c_write(0x80 | 0x0C);  // Data0 low byte
    i2c_start();
    i2c_write((TSL2561_ADDR << 1) | 1);
    low = i2c_read(1);
    high = i2c_read(0);
    i2c_stop();

    unsigned int ch0 = (high << 8) | low;
    return (float)ch0 * 0.034;  // Simplified lux calculation
}


void lcd_init(void) {
    // Reset RB6 to start with it turned off
    TRISBbits.TRISB6 = 0; // Set RB6 as output
    LATBbits.LATB6 = 0; // Set RB6 low
    delay_ms(10); // Keep RB6 low for 10 ms
    LATBbits.LATB6 = 1; // Set RB6 high to turn it on

    delay_ms(10); // Keep RB6 low for 10 ms
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

void lcd_cmd(char command) {
    LATBbits.LATB5 = 1;
    // Implement I2C write command
    I2C1CONbits.SEN = 1; // Initiate START condition
    while (I2C1CONbits.SEN); // Wait for START condition to complete
    I2C1TRN = (LCD_I2C_ADDRESS << 1) & 0xFE; // Send address with write bit
    wait_for_ack();
    I2C1TRN = 0x00; // Control byte: Co = 0, D/nC = 0 (command)
    wait_for_ack();
    I2C1TRN = command; // Send command
    wait_for_ack();
    I2C1CONbits.PEN = 1; // Initiate STOP condition
    while (I2C1CONbits.PEN); // Wait for STOP condition to complete
    LATBbits.LATB5 = 0;
}

void lcd_setCursor(char x, char y) {
    char address = 0x80 + (y * 0x20) + x; // Calculate address based on row and column
    lcd_cmd(address);
}

void lcd_printChar(char myChar) {
    LATBbits.LATB5 = 1;
    // Implement I2C write data
    I2C1CONbits.SEN = 1; // Initiate START condition
    while (I2C1CONbits.SEN);
    I2C1TRN = (LCD_I2C_ADDRESS << 1) & 0xFE; // Send address with write bit
    wait_for_ack();
    I2C1TRN = 0x40; // Control byte: Co = 0, D/nC = 1 (data)
    wait_for_ack();

    I2C1TRN = myChar; // Send character data
    wait_for_ack();
    I2C1CONbits.PEN = 1; // Initiate STOP condition
    while (I2C1CONbits.PEN);
    LATBbits.LATB5 = 0;
}

void lcd_printStr(const char *str) {
    while (*str) {
        lcd_printChar(*str++);
    }
}


void initBuffer(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
    sum = 0;
    count = 0;
}

void putVal(int newValue) {
    buffer[front] = newValue;
    front = (front + 1) % BUFFER_SIZE;
    back = (back + 1) % BUFFER_SIZE;
}

int getAvg(void) {
    long tempSum = 0; // Use long to prevent overflow
    int tempback = back; // Use long to prevent overflow
    int samplesCounted = 0; // Counter for traversing samples
    while (tempback != front) {
        tempSum += buffer[tempback]; // Add the value at the current index to the sum
        tempback = (tempback + 1) % BUFFER_SIZE;

        samplesCounted++; // Increment the counter
    }
    return (int) (tempSum / SAMPLES); // Return the average as an integer
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {
    int adcValue = ADC1BUF0;
    putVal(adcValue);
    IFS0bits.AD1IF = 0;
}


void initADC(void) {
    // Configure AN0 (RA0) as analog input
    TRISAbits.TRISA0 = 1; // Set RA0 (AN0) as input
    AD1PCFGbits.PCFG0 = 0; // Configure AN0 as an analog input (digital disabled)
    // ADC Control Register 1 (AD1CON1)
    AD1CON1bits.FORM = 0; // Data Output Format: Integer
    AD1CON1bits.SSRC = 2; // Conversion Trigger Source: Timer3 compare match
    AD1CON1bits.ASAM = 1; // Automatic sampling begins after conversion ends
    // ADC Control Register 2 (AD1CON2)
    AD1CON2bits.VCFG = 0; // Voltage Reference Configuration: AVdd and AVss
    AD1CON2bits.CSCNA = 0; // Do not scan inputs
    AD1CON2bits.SMPI = 1; // Interrupt after every sample/convert sequence
    AD1CON2bits.ALTS = 0; // Always use MUX A
    // ADC Control Register 3 (AD1CON3)
    AD1CON3bits.ADRC = 0; // ADC Clock derived from system clock
    AD1CON3bits.SAMC = 1; // Auto-sample time: 31 TAD
    AD1CON3bits.ADCS = 2; // ADC Conversion Clock Select (TAD = 64 * Tcy)
    // Input Channel Selection (AD1CHS)
    AD1CHSbits.CH0SA = 0; // Positive input is AN0
    AD1CHSbits.CH0NA = 0; // Negative input is VR- (AVss)

    // Analog Scan Input Selection (AD1CSSL)
    AD1CSSL = 0x0000; // Disable input scanning
    // Enable ADC Module and Interrupt
    IEC0bits.AD1IE = 1; // Enable ADC interrupt
    IFS0bits.AD1IF = 0; // Clear ADC interrupt flag
    AD1CON1bits.ADON = 1; // Turn on ADC
}

void lcd_scrollMessage(const char *msg, int row) {
    int len = strlen(msg);  // Get the length of the message
    char buffer[16 + 1];    // Buffer for one line (16 characters + null terminator)
    memset(buffer, ' ', sizeof(buffer) - 1); // Fill buffer with spaces
    buffer[16] = '\0';      // Null-terminate the buffer

    for (int i = 0; i < len + 16; i++) {
        // Create a sliding window for the message
        for (int j = 0; j < 16; j++) {
            if (i + j < len) {
                buffer[j] = msg[(i + j) % len]; // Add character from message
            } else {
                buffer[j] = ' '; // Add spaces when out of message bounds
            }
        }

        lcd_setCursor(0, row);   // Set cursor to the specified row
        lcd_printStr(buffer);   // Print the current sliding window
        delay_ms(75);          // Adjust delay for scrolling speed
    }
}

// Example of a correct function definition

void initTimer3(unsigned int K) {
    T3CON = 0; // Timer settings (Enable Timer3, Prescaler 1:1)
    T3CONbits.TCKPS = 3;
    PR3 = (62500 / K) - 1;
    T3CONbits.TON = 1;
}

// Function to read ADC value

unsigned int readSensorADC(void) {
    AD1CON1bits.SAMP = 1; // Start sampling
    while (!AD1CON1bits.DONE); // Wait until conversion is complete
    return ADC1BUF0; // Return the ADC result
}

//int main(void) {
//    CLKDIVbits.RCDIV = 0;  // Set clock divider to 0
//    i2c_init();            // Initialize I2C
//    lcd_init();            // Initialize LCD
//    initADC();             // Initialize ADC for moisture sensor
//
//    while (1) {
//        unsigned int adcValue = readSensorADC();  // Read ADC value
//        float voltage = (adcValue * VREF) / (ADC_RESOLUTION - 1);  // Calculate voltage
//
//        // Format and display voltage without snprintf()
//        lcd_setCursor(0, 0);  // Line 1
//        lcd_printStr("Voltage: ");
//        lcd_printChar('0' + (int)voltage);  // Print integer part
//        lcd_printChar('.');                 // Print decimal point
//        lcd_printChar('0' + (int)(voltage * 10) % 10);  // Print first decimal
//        lcd_printChar('0' + (int)(voltage * 100) % 10); // Print second decimal
//
//        delay_ms(500);  // Delay for stability
//    }
//
//    return 0;
//}






int main(void) {
    CLKDIVbits.RCDIV = 0;
    i2c_init();      // Initialize I2C
    lcd_init();      // Initialize LCD
    initADC();       // Initialize ADC
    initTimer3(128); // Initialize Timer3
    initTSL2561();   // Initialize the TSL2561 sensor
    
    const char *moistureMessage;
    const char *lightMessage;
    int moistureOffset = 0;
    int lightOffset = 0;
    char moistureBuffer[17];  // Buffer for scrolling moisture message
    char lightBuffer[17];     // Buffer for scrolling light message

    while (1) {
        // Determine Moisture Message
        unsigned int adcValue = readSensorADC();  // Read raw ADC value
        float voltage = (adcValue * VREF) / (ADC_RESOLUTION - 1);  // Convert ADC value to voltage

        if (voltage > V_THRESHOLD) {
            moistureMessage = "Moisture Detected       Moisture Detected       ";  // Add spaces for scrolling
                       

        } else {
            moistureMessage = "No Moisture Detected    No Moisture Detected    ";  // Add spaces for scrolling
                      

        }
        // Determine Light Message
        unsigned int lightLevel = readLightLevel();  // Read light level from the sensor

        if (lightLevel < LIGHT_THRESHOLD) {
            lightMessage =   "Insufficient Light        Insufficient Light    ";  // Add spaces for scrolling
        } else {
            lightMessage =   " Sufficient Light         Sufficient Light      ";  // Add spaces for scrolling
        }

        // Update Scrolling Buffers
        for (int i = 0; i < 16; i++) {
            // Fill moisture buffer for line 1
            moistureBuffer[i] = moistureMessage[(moistureOffset + i) % strlen(moistureMessage)];
            // Fill light buffer for line 2
            lightBuffer[i] = lightMessage[(lightOffset + i) % strlen(lightMessage)];
        }
        moistureBuffer[16] = '\0';  // Null-terminate buffer
        lightBuffer[16] = '\0';     // Null-terminate buffer

        // Update LCD
        lcd_setCursor(0, 0);  // Line 1
        lcd_printStr(moistureBuffer);
        lcd_setCursor(0, 1);  // Line 2
        lcd_printStr(lightBuffer);

        // Update Offsets for Scrolling
        moistureOffset = (moistureOffset + 1) % strlen(moistureMessage);
        lightOffset = (lightOffset + 1) % strlen(lightMessage);

        delay_ms(50);  // Adjust delay for scrolling speed
        
        
        
    }

    return 0;
}




