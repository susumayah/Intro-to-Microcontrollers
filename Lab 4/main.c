#include "xc.h"
#include "servo.h"
#include "stdlib.h"

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

void setServo(double);
volatile uint32_t overflow = 0;
volatile uint32_t Currentoverflow = 0;
volatile uint32_t clicks = 0;
volatile uint32_t state = 1; // assume button is not pressed
volatile int buffSize = 0, write = 0, read = 0;
volatile uint32_t first = 0, second = 0, Third = 0;
volatile uint32_t curPeriod = 0, prevPress = 0, currPress = 0;
volatile uint32_t diffTest1 = 0, diffTest2 = 0, diffTest3 = 0;

#define circBuffSize 3

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    overflow++;
    _T2IF = 0;
}

void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void) {
   
   _IC1IF = 0;
   uint32_t time;
   while(_ICBNE)
   {
        currPress = (uint32_t)(IC1BUF + ((uint32_t)PR2)*overflow);
        curPeriod = (currPress - prevPress);
        time = curPeriod;
        if (time < 125) // debouncing check with the value of 125 being the 2 ms debounce delay with timer 2 with PRE 1:256
        {
            return;  
        } else // value
        {
            prevPress = currPress;

            if (state == 1) // value 15625 for PRE 1:256 for timer 2 gives 250 ms
            {                                 // check to see if the state was pressed or not within a 250 ms window....
                state = 0; // sets the state to be 1 to indicate that the button was pressed event
                addBuffer(currPress); // sends value to the buffer
                // test code here//
                //clicks++;
            }
            else if(state == 0) // checks to see is state was pressed
            {
                state = 1; // changes state to be button released
                return;
            }
        }
   }
   
}

////--- Buffer functions ---////
volatile uint32_t buffer[circBuffSize];
void addBuffer(uint32_t x)
{
    //if (buffSize < circBuffSize)
    //{
          buffer[write++] = x;
          write = write % circBuffSize;
          //++buffSize;
    //}
    //Otherwise get rid of the data.
}
uint32_t getBuffer()
{
    uint32_t returnValue = buffer[read++];
    read = read % circBuffSize;
    --buffSize;
    return returnValue;
}
int emptyBuffer(void) {
    if (buffSize <= 0) {
        buffSize=0;
        return 1;
    } else {
        return 0;
    }
}
////--- Buffer functions ---////

void initPushButton(void) {
    //The IC1 module
    //should be set up and configured to use Timer 2 and capture falling edges.
   
    T2CON = 0;
    PR2 = 62499; // sets period to 65565 count duration // 65535
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1; //enable t2 interrupt
    IPC1bits.T2IP = 3; //interrupt priority
    T2CONbits.TCKPS = 0b11; // pre-scaler is 1:256
   
    CNPU2bits.CN22PUE = 1; //Pull up on RB8 (button input))
   
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8;  // IC1 on RP8
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
   
    IC1CON = 0; // Turn off and reset internsl state of IC1
    IC1CONbits.ICTMR = 1; //timer 2 for ic1 (i.e. uses Timer 2 for capture source)
    IEC0bits.IC1IE = 1; //enable ic1 interrupt
    IC1CONbits.ICM = 0b001; //capture every rising edge turn on
    IC1CONbits.ICI = 0b00; //interrupt on every capture event
    IPC0bits.IC1IP = 4; //interrupt priority
    _IC1IF = 0;
    _T2IF = 0;
   
    T2CONbits.TON = 1; //timer 2 enabled
     
}

void delay(unsigned int ms) {
    int i;
    for (i = 0; i < ms; i++) {
        asm("repeat #15993");
        asm("nop");
    }
    return;
}

void setup(void) {
    CLKDIVbits.RCDIV = 0; //clock setup thing
    AD1PCFG = 0x9fff; //sets all pins to digital I/O
    TRISBbits.TRISB6 = 0; //servo pin output
    TRISBbits.TRISB8 = 1; //button pin input;
   
    initServo();
    initPushButton();
}

int main(void) {
    setup();
    setServo(0); // sets the servo to the zero position
   
    uint32_t lastTime = 0;
    uint32_t CurrnetTime=0;
    const uint32_t cycles = 125000; // 2s threshold in instruction cycles
   
   
    int i = 0;
    while (i < circBuffSize) {
        buffer[i] = 0;
        i++;
    }

   
    while(1)
    {
       
       //if (emptyBuffer() == 0)
            //continue;
        //Buffer is not empty, keep going
        //first = getBuffer();
       
        //if(buffSize >= 3)
        {
            first = buffer[write];
            second = buffer[(write + 1)%circBuffSize]; // stores result of first into second
            Third = buffer[(write + 2)%circBuffSize]; // stores the result of second into thrid

            diffTest1 = second - first;
            diffTest2 = Third - second;
            //diffTest3 = abs(first - Third);
       
            //if(diffTest1 < 15625 && diffTest2 < 15625 && clicks == 3 ) // if(diffTest1 > 250) --> 250 is .25 ms -->  15625
            if(diffTest1 < 15625 && diffTest2 < 15625 ) // if(diffTest1 > 250) --> 250 is .25 ms -->  15625
            {
                 //buffSize -=3;
                 //read = (read + 3)%circBuffSize;
                 //Currentoverflow  = overflow;
                 lastTime = Third;
                 
                 //TMR2 = 0;
                 //clicks = 0;
               
            }
            //else
            //{
            //    buffSize--;
            //    read = (read+1)%circBuffSize;
            //    TMR2 = 0;
               //clicks = 0;
                               
            //}
        }
       
        // Check if the current time has exceeded the threshold
        CurrnetTime = (uint32_t)(IC1BUF + ((uint32_t)PR2)*overflow);

        if (CurrnetTime < lastTime + cycles) {
            setServo(0.5); // Sets the servo to the zero position
        } else {
            setServo(1);
        }
    }
   
    return 0;
}
