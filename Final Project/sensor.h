/* 
 * File:   sensor.h
 * Author: sumay
 *
 * Created on December 7, 2024, 8:19 PM
 */
#ifndef SENSOR_H
#define SENSOR_H

#include <xc.h>

// Constants for light and moisture detection
#define V_THRESHOLD 2.0  // Voltage threshold for moisture detection
#define VREF 3.3          // Reference voltage for ADC
#define ADC_RESOLUTION 1024
#define THRESHOLD 50  // Lux threshold for sufficient light
#define TSL2561_ADDR 0x39  // Default I2C address for the TSL2561 sensor
#define COMMAND_BIT 0x80   // Command bit for register access
#define TSL2561_ON 0x03    // Power ON command
#define TSL2561_OFF 0x00   // Power OFF command
#define GAIN_16X 0x02      // High gain for low light
#define INTEGRATION_TIME 0x00 // Short integration time for fast sampling
#define _XTAL_FREQ 16000000
#define FCY (_XTAL_FREQ / 2)
// Function prototypes
// TSL2561 Sensor Functions
void tsl2561_init(void);
float tsl2561_readLux(void);
float readLightLevel(void); // Read light level from the TSL2561
unsigned int readSensorADC(void); // Read ADC value from moisture sensor
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);


#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H */


