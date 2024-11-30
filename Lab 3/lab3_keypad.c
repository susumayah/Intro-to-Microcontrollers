#include "xc.h"
#include "lab3_keypad.h"



void initKeyPad(void){
    TRISB = 0b0000000000000000; // Port B (RB12-RB15) = outputs
    TRISA = 0b0000000000001111; // Port A (RA0-RA3) = inputs
    CNPU2bits.CN29PUE = 1;  // turn on pull-up for pin 10
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN3PUE = 1;
    CNPU1bits.CN2PUE = 1;
}

void elmi0034Test7seg() {

    showChar7seg('0', 0);
    delay(1000);
    showChar7seg('1', 1);
    delay(1000);
    showChar7seg('2', 0);
    delay(1000);
    showChar7seg('3', 1);
    delay(1000);
    showChar7seg('4', 0);
    delay(1000);
    showChar7seg('5', 1);
    delay(1000);
    showChar7seg('6', 0);
    delay(1000);
    showChar7seg('7', 1);
    delay(1000);
    showChar7seg('8', 0);
    delay(1000);
    showChar7seg('9', 1);
    delay(1000);
    showChar7seg('A', 0);
    delay(1000);
    showChar7seg('B', 1);
    delay(1000);
    showChar7seg('C', 0);
    delay(1000);
    showChar7seg('D', 1);
    delay(1000);
    showChar7seg('E', 0);
    delay(1000);
    showChar7seg('F', 1);
    delay(1000);
   
}

 char readKeyPadRAW(void){
      LATBbits.LATB12 = 0;  
      LATBbits.LATB13 = 1;
      LATBbits.LATB14 = 1;  
      LATBbits.LATB15 = 1;                            

    delay(1);
    char num =' ';
    if(PORTAbits.RA3 == 0){
        num = '1'; //Display 1
    }
    else if(PORTAbits.RA2 == 0){
        num = '2'; //Display 2
    }
    else if(PORTAbits.RA1 == 0){
        num = '3'; //Display 3
    }
    else if(PORTAbits.RA0 == 0){
        num = 'A'; //Display A
    }
   
      LATBbits.LATB12 = 1;  
      LATBbits.LATB13 = 0;
      LATBbits.LATB14 = 1;  
      LATBbits.LATB15 = 1;    delay(1);
    if(PORTAbits.RA3 == 0){
        num = '4'; //Display 4
    }
    else if(PORTAbits.RA2 == 0){
        num = '5'; //Display 5
    }
    else if(PORTAbits.RA1 == 0){
        num = '6'; //Display 6
    }
    else if(PORTAbits.RA0 == 0){
        num = 'b'; //Display B
    }
   
     LATBbits.LATB12 = 1;  
     LATBbits.LATB13 = 1;
     LATBbits.LATB14 = 0;  
     LATBbits.LATB15 = 1;    
     delay(1);
    if(PORTAbits.RA3 == 0){
        num = '7'; //Display 7
    }
    else if(PORTAbits.RA2 == 0){
        num = '8'; //Display 8
    }
    else if(PORTAbits.RA1 == 0){
        num = '9'; //Display 9
    }
    else if(PORTAbits.RA0 == 0){
        num = 'C'; //Display C
    }
   
      LATBbits.LATB12 = 1;  
      LATBbits.LATB13 = 1;
      LATBbits.LATB14 = 1;  
      LATBbits.LATB15 = 0;    
      delay(1);
    if(PORTAbits.RA3 == 0){
        num = 'E'; //Display E
    }
    else if(PORTAbits.RA2 == 0){
        num = '0'; //Display 0
    }
    else if(PORTAbits.RA1 == 0){
        num = 'F'; //Display F
    }
    else if(PORTAbits.RA0 == 0){
        num = 'D'; //Display D
    }
   
    return num;
}


