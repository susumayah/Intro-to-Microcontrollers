#include "xc.h"
#include "lab3_keypad.h"
#include "lab3_LED.h"
#include "elmi_lab3_core_v002.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled,
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
extern void wait_1ms(void);
extern void elmi0034Test7seg(void);  // Add this if the function is defined elsewhere



void delay(int num){
    while(num){
        num--;
        wait_1ms();
       
    }
}

int main(void) {
   
    init7seg();
    initKeyPad();
    char key = ' ';
    char left=' ';
    char right=' ';
    while(1){
    key=readKeyPadRAW();
    delay(3);
    if (key != ' ') {
        left=right;  
        right=key;
    }

   
    showChar7seg(left,0);   // Show Left on LATB
    delay(10);      // Delay to hold the output
    showChar7seg(right,1);   // Show Left on LATB
     delay(10);

        //elmi0034Test7seg();
        //showChar7seg(readKeyPadRAW(), 1);
       
       
    }
   
    return 0;
}