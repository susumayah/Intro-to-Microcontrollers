
#include <xc.h>  // Include appropriate header file for the microcontroller

#include "elmi_lab2b_core_v002.h"

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

#define PERIOD 5  // Time in milliseconds per frame

#define packColor(r, g, b) ((uint32_t)(r) << 16 | (uint32_t)(g) << 8 | (uint32_t)(b))

#define getR(color) ((color >> 16) & 0xFF)

#define getG(color) ((color >> 8) & 0xFF)

#define getB(color) (color & 0xFF)


void writeColor(int r, int g, int b) {

    int i;

    for (i = 7; i >= 0; i--) {

        (r & (1 << i)) ? write_1() : write_0();

    }

    for (i = 7; i >= 0; i--) {

        (g & (1 << i)) ? write_1() : write_0();

    }

    // Red component

    

    // Blue component

    for (i = 7; i >= 0; i--) {

        (b & (1 << i)) ? write_1() : write_0();

    }

    wait_100us();

}


void delay(int delay_in_ms) {

    for (int i = 0; i < delay_in_ms; i++) {

        wait_1ms();

    }

}


uint32_t Wheel(unsigned char WheelPos) {

    WheelPos = 255 - WheelPos;  // Reverse the wheel position

    if (WheelPos < 85) {

        return packColor(255 - WheelPos * 3, 0, WheelPos * 3);

    } else if (WheelPos < 170) {

        WheelPos -= 85;

        return packColor(0, WheelPos * 3, 255 - WheelPos * 3);

    } else {

        WheelPos -= 170;

        return packColor(WheelPos * 3, 255 - WheelPos * 3, 0);

    }

}

void writePacCol(uint32_t PackedColor) {

    unsigned char red = getR(PackedColor);

    unsigned char green = getG(PackedColor);

    unsigned char blue = getB(PackedColor);

    writeColor(red, green, blue);

}

void animateWheel(int period_ms) {

    unsigned char wheelPosition = 0;

    while (1) {

        uint32_t color = Wheel(wheelPosition);

        writePacCol(color);

        delay(period_ms);  

        wheelPosition++;  

        if (wheelPosition == 255) {  

            wheelPosition = 0;

        }

    }

}



void animateColors(void) {

    unsigned char frameNumber = 0;

    uint32_t color;


    while (1) {

        if (frameNumber < 85) {

            color = packColor(255 - frameNumber * 3, 0, frameNumber * 3);

        } else if (frameNumber < 170) {

            frameNumber -= 85;

            color = packColor(0, frameNumber * 3, 255 - frameNumber * 3);

        } else {

            frameNumber -= 170;

            color = packColor(frameNumber * 3, 255 - frameNumber * 3, 0);

        }


        writePacCol(color);

        delay(PERIOD);

        frameNumber = (frameNumber + 1) % 256;  // Wrap around at 255

    }

}




void delay_1sec(int n) {

    for (int i=0; i<n; i++){

        wait_1ms();  // 1 second delay     

    }

}


int main(void) {

    AD1PCFG = 0xFFFF;  // Set all pins to digital mode

    TRISA = 0xFFFE;    // Set RA0 pin to output

    LATA = 0x0000;     // Set all PORTA pins low


    while(1) {

       // writeColor(0,0,128);

        //animateColors();

        animateWheel(2);

    }


    return 0;

}