#pragma config ICS = PGx1 // Emulator EMUC1/EMUD1 pins shared with PGC1/PGD1
#pragma config FWDTEN = OFF // Disable Watchdog Timer
#pragma config GWRP = OFF // General Code Segment Write Protect
#pragma config GCP = OFF // General Code Segment Code Protect
#pragma config JTAGEN = OFF // Disable JTAG Port
#pragma config I2C1SEL = PRI // Default SCL1/SDA1 pins
#pragma config IOL1WAY = OFF // IOLOCK can be changed
#pragma config OSCIOFNC = ON // CLKO functions as I/O pin
#pragma config FCKSM = CSECME // Clock switching enabled, Fail-Safe Clock Monitor
enabled
#pragma config FNOSC = FRCPLL // Fast RC Oscillator with PLL
#include "circBuff.h"
#include "lab5_lcd.h"
#include <stdio.h>

#define ADC_RESOLUTION 1024
#define _XTAL_FREQ 16000000 // Define the crystal frequency for delay functions
#define FCY (_XTAL_FREQ / 2)
#define wait_for_ack() _MI2C1IF = 0; while (_MI2C1IF == 0 || I2C1STATbits.TRSTAT == 1);
#define VREF 3.3 // Reference voltage (adjust as needed for your system)
#define BUFFER_SIZE 1024
#define SAMPLES 128
static int buffer[BUFFER_SIZE];
static int front = SAMPLES;
static int back = 0;
static long sum = 0;
static int count = 0;
void i2c_init(void) {
I2C1BRG = 157; // Set baud rate generator for 100kHz with Fcy = 16MHz
I2C1CONbits.I2CEN = 1; // Enable I2C module
IFS1bits.MI2C1IF = 0; // Clear interrupt flag
}
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
void lcd_scrollMsg(const char *msg) {
int len = strlen(msg);
char buffer[17]; // Assume 16 character width for display + null terminator
memset(buffer, ' ', 16); // Fill buffer with spaces
buffer[16] = '\0';
for (int i = 0; i < len + 16; i++) {
// Copy substring of message to buffer, shifting by one character each iteration
for (int j = 0; j < 16; j++) {

if (i + j < len) {
buffer[j] = msg[i + j];
} else {
buffer[j] = ' '; // Pad with spaces when message is shorter than display width
}
}
lcd_setCursor(0, 0); // Set cursor to the start of the first row
lcd_printStr(buffer); // Print the current buffer to the display
_delay_ms(100); // Delay for scrolling effect (adjust as needed)
}
}
void _delay_ms(unsigned int ms) {
unsigned long cycles = (unsigned long)ms * (FCY / 1000);
while (cycles > 0) {
__asm__ volatile ("nop");
cycles--;
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
tempback=(tempback+ 1) % BUFFER_SIZE;

samplesCounted++; // Increment the counter
}
return (int)(tempSum / SAMPLES); // Return the average as an integer
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {
int adcValue = ADC1BUF0;
putVal(adcValue);
IFS0bits.AD1IF = 0;
}
//void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
// float avgVoltage = (float)getAvg() * (VREF / ADC_RESOLUTION);
// displayVoltage(avgVoltage);
// IFS0bits.T3IF = 0;
//}
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

// Example of a correct function definition
void initTimer3(unsigned int K) {
T3CON = 0; // Timer settings (Enable Timer3, Prescaler 1:1)
T3CONbits.TCKPS=3;
PR3= (62500/K)-1;
T3CONbits.TON=1;
}