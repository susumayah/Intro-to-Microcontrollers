    .section .text
    .global _i2c_start
    .global _i2c_stop
    .global _i2c_write

// I2C Start Condition
_i2c_start:
    bclr    I2C1CON, #SEN          ; Set SEN bit to initiate Start condition
_wait_start:
    btss    I2C1CON, #SEN          ; Wait until SEN is cleared
    bra     _wait_start
    return

// I2C Stop Condition
_i2c_stop:
    bclr    I2C1CON, #PEN          ; Set PEN bit to initiate Stop condition
_wait_stop:
    btss    I2C1CON, #PEN          ; Wait until PEN is cleared
    bra     _wait_stop
    return

// I2C Write Byte
// Input: W0 - Byte to write
_i2c_write:
    mov     W0, I2C1TRN            ; Move W0 to transmit register
_wait_write:
    btss    I2C1STAT, #TRSTAT      ; Wait for transmit to complete
    bra     _wait_write
    return


