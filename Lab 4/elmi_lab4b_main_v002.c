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

#define PRESS 1
#define RELEASE 0
#define DEBOUNCE 130
#define BUFFERSIZE 3
#define BUTTON PORTBbits.RB8

#include "elmi_lab4b_main_v002.h"
#include "xc.h"
#include <libpic30.h>


void initpushbutton(void) {
    AD1PCFG = 0x9FFF;
    TRISBbits.TRISB8 = 1;
    CNPU2bits.CN22PUE = 1;
    T2CON = 0x0030; //256 prescale
    TMR2 = 0;
    PR2 = 62499;
    _T2IE = 1;
    _T2IF = 0;

    IC1CON = 0;
    IC1CONbits.ICM = 0b001;
    IC1CONbits.ICTMR = 1;
    IC1CONbits.ICBNE = 0; //reset everything, buffer currently empty
    _IC1IF = 0;
    _IC1IE = 1;

    //PPS: bind IC1 to RP8/RB8
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8; // uses Pin RP8 for input capture (table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    T2CONbits.TON = 1;
}
// Function Prototypes
void initservo(){
    AD1PCFG = 0x9FFF;
    CLKDIVbits.RCDIV = 0;
    TRISB = 0xFFFF;
    TRISBbits.TRISB6 = 0;
    T3CON = 0;
    T3CONbits.TCKPS1 = 0;
    T3CONbits.TCKPS0 = 1;   //prescale = 8
    TMR3 = 0;
    PR3 = 39999;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;  // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    OC1CON = 0;
    OC1R = 2000;
    OC1RS = 2000;
    OC1CONbits.OCTSEL = 1;
    OC1CONbits.OCM = 0b110;
    T3CONbits.TON = 1;
}

void setservo(int val){
    OC1RS = val;
}


int duty_1ms = 1250;        //servo -90 degrees
volatile unsigned int count = 0; // counts the rollovers 

void delay(int num){ //wait 1ms
    while(num){
        num--;
        asm("repeat #15993");
        asm("nop");
       
    }
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void){
    count = count +1 ;   
    _T2IF = 0;
}

int curstate;
int prevstate;

volatile long int interruptvar = 0;
volatile long int ic1time;
volatile long int lastevent = 0;

unsigned long buffer[BUFFERSIZE];


void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void){
    IFS0bits.IC1IF = 0;
    ic1time = TMR2 + ((long int)(PR2 + 1) * count);   //capture current overall timer time
    
        if (ic1time - lastevent > DEBOUNCE){
            lastevent = ic1time;
            if (!prevstate){    //checks if not pressed
                prevstate = PRESS;
                bufferadd(ic1time);
            } else {
                prevstate = RELEASE;
            }
        }
}

volatile int buffercount = 0;
volatile int read = 0;
volatile int write = 0;
void bufferadd(volatile long int num){
    if (buffercount < 3){
        buffer[write] = num;
        buffercount++;
        write++;
        write %= BUFFERSIZE;
    }
}

long int buffershift(void){
    long int val;
    if (buffercount > 0){
        val = buffer[read++];
        read %= BUFFERSIZE;
        buffercount--;
    }
    return val;
}

int buffersize(void){
    return buffercount;
}

int main(){
    initservo();
    initpushbutton();
    
    long int event1 = 0;
    long int event2 = 0;
    long int event3 = 0;
    long int trash;
    long int currenttime;
    long int successfulclicktime;
    
    long int diff1;
    long int diff2;
    
    setservo(duty_1ms);    
    while (1){  

        currenttime = TMR2 + ((long int)(PR2 + 1) * count);
        if (buffersize() >= 3){
            
            event1 = buffershift();
            event2 = buffershift();
            event3 = buffershift();
            
            diff1 = event2 - event1;
            diff2 = event3 - event2;
                
            if (diff1 < 15625UL && diff2 < 15625UL){
                setservo(4000);
                successfulclicktime = currenttime;
            }else{ 
                trash = buffershift();
            }   
        }      
        //---------
        
//        if (buffersize() >= 2){
//            
//            event1 = buffershift();
//            event2 = buffershift();
////            event3 = buffershift();
//            
//            diff1 = event2 - event1;
////            diff2 = event3 - event2;
//                
//            if (diff1 < 15625UL){
//                setservo(4000);
//                successfulclicktime = currenttime;
//            }else{ 
//                trash = buffershift();
//            }   
//        }  
        
        //----

        
        if ((currenttime - successfulclicktime) > 125000UL){  //2 seconds have passed
            setservo(duty_1ms);
        }
    }
    return 0;
}