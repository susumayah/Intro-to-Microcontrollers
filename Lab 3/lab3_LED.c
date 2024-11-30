#include "xc.h"
#include "lab3_LED.h"

void init7seg(void){
    CLKDIVbits.RCDIV = 0;  // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            // Set all pins to digital I/O
    TRISB = 0b0000000000000000;  // Set port b to outputs
    LATB = 0b0000000000000000;  // Set LAT B to outputs
}



void showChar7seg(char myChar, int myDigit){
//where myChar is the character being displayed
//where myDigit indicates LEFT (1) or RIGHT (0)
   
    // A=RB9, B=RB8, C=RB7, D=RB6, E=RB5, F=RB4, G=RB3,
    LATB &= 0xF003;

   
    if(myDigit == 0){
        switch(myChar){
            case '0':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;
                break;
            case '1':        
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 1;
                break;
            case '2':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '3':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '4':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 1;                              
                break;
            case '5':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case '6':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case '7':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '8':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '9':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case 'A':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case 'b':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 1;                              
                break;
            case 'C':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'd':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 1;                              
                break;
            case 'E':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'F':
                LATBbits.LATB11 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'L':
                LATBbits.LATB11 = 0;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
        }
       
    }
    else{
    switch(myChar){
            case '0':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;
                break;
            case '1':        
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 1;
                break;
            case '2':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '3':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '4':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 1;                              
                break;
            case '5':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case '6':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case '7':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '8':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case '9':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case 'A':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
            case 'b':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 1;                              
                break;
            case 'C':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'd':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 1;                              
                break;
            case 'E':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'F':
                LATBbits.LATB10 = 1;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB7 = 1;
                LATBbits.LATB8 = 1;
                LATBbits.LATB9 = 0;                              
                break;
            case 'L':
                LATBbits.LATB11 = 0;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 0;
                LATBbits.LATB7 = 0;
                LATBbits.LATB8 = 0;
                LATBbits.LATB9 = 0;                              
                break;
        }
    }
}
