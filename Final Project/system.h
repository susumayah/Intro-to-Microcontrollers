/* 
 * File:   system.h
 * Author: sumay
 *
 * Created on December 7, 2024, 7:38 PM
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

// Constants
#define V_THRESHOLD 2.0
#define VREF 3.3
#define ADC_RESOLUTION 1024

// Function Prototypes
void initLEDs(void);  // Initialize the LEDs
void setLEDs(int moistureDetected);  // Set LEDs based on detection
int isMoistureDetected(void);  // Check if moisture is detected
void updateDisplay(int moistureDetected);  // Update LCD display
void initSystem(void);  // Initialize all systems

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H */


