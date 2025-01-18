/*
 * File:   elmi_lab4_main_v002.c
 * Author: sumay
 *
 * Created on October 30, 2024, 11:39 AM
 */
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL

#include "xc.h"

// Function to create delay in ms
void msDelay(int ms) {
    for (int i = 0; i < ms; i++){
        __delay32(16000);  // Assuming a 16 MHz clock, 1 ms delay
    }
}

// Initialize Servo with Timer 3 and Output Compare 1 (OC1)
void initServo(void) {
    TRISBbits.TRISB6 = 0;
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;  // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS

    T3CON = 0x0000;          // Stop Timer 3
    T3CONbits.TCKPS = 0b01;  // Set prescaler to 1:8
    PR3 = 40000 - 1;         // Set period (20ms / (62.5ns* 8) = 40000 counts)
    TMR3 = 0;                // Clear Timer 3

    OC1CON = 0x0000;         // Turn off OC1 for setup
    OC1R = 2000;             // Initial pulse width (center position)
    OC1RS = 2000;            // Shadow register for OC1R
    OC1CONbits.OCTSEL = 1;   // Use Timer 3 as the clock source
    OC1CONbits.OCM = 0b110;  // PWM mode without faults
    T3CONbits.TON = 1;       // Start Timer 3
}

// Set Servo Position based on pulse width
void setServo(int val) {
    OC1RS = val;  // Update the pulse width in the shadow register
}

// Initialize Push Button and Timer 2 for overflow handling
void initPushButton(void) {
    TRISBbits.TRISB8 = 1;         // Configure RB0 as input
    CNPU2bits.CN22PUE = 1;         // Enable pull-up resistor on RB0
    
    T2CON = 0;
    T2CON = 0x0030;               // Stop Timer2 and set prescaler to 1:256
    PR2 = 62500 - 1;              // Set period to 1 second
    TMR2 = 0;                     // Clear Timer2

    IFS0bits.T2IF = 0;            // Clear interrupt flag
    IEC0bits.T2IE = 1;            // Enable Timer 2 interrupt
    T2CONbits.TON = 1;            // Start Timer2
}

volatile long overflowCount = 0;
volatile int curState = 1;
volatile int prevState = 1;
volatile long time_press1 = 0;
volatile long time_press2 = 0;
volatile long lastEventTime = 0;

// Timer 2 ISR for overflow handling
void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;            // Clear Timer 2 interrupt flag
    overflowCount++;              // Increment overflow counter

}

// Get current time with overflow count
long getCurrentTime() {
    return TMR2 + (overflowCount * 62500L);  // 32 bits
}

int main(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9FFF;  // Set all analog pins to digital
    initServo();
    initPushButton();
    setServo(4000);

    while (1) {
        
//        setServo(2000);
//        msDelay(2000);
//        setServo(4000);
//        msDelay(2000);
        curState = PORTBbits.RB8;  // read button state

        // Detect a button press 
      if (curState == 0 && prevState == 1) {
         time_press1 = getCurrentTime();      // Record current press time
           if (time_press1 - time_press2 < 15625) {  // Double click detected
               setServo(3000);  // Move servo to -90 degrees
              lastEventTime = getCurrentTime();  // Reset inactivity timer
         }
           time_press2 = time_press1;          // Shift previous press time to current

        }

        // Check for timeout to reset servo position
       if (getCurrentTime() - lastEventTime > 125000) {
           setServo(5000);  // Reset servo position to 0
       }
//
        prevState = curState;  // Update previous state with current state
        msDelay(2);  // 2 ms debounce delay
    }
    return 0;
 }
    

