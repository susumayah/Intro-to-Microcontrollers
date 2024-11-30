/* 
 * File:   lab3_LED.h
 * Author: sumay
 *
 * Created on October 22, 2024, 12:37 PM
 */

#ifndef LAB3_LED_H
#define	LAB3_LED_H

#ifdef	__cplusplus
extern "C" {
#endif

void init7seg(void);
void showChar7seg(char myChar, int myDigit);
void loop(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LAB3_LED_H */

