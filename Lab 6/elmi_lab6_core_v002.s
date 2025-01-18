 
.include "xc.inc"

.text  ; Directs the assembler to put the following data in ROM (program memory)

; Global statement for external visibility
.global _write_0, _write_1, _wait_100us, _wait_1ms

; Function to write '0' (Example of GPIO manipulation)
_write_0:
    inc LATA          ; Set data pin high (for example RA0)
    nop
    nop
    clr LATA          ; Set data pin low
    repeat #2          ; Longer delay to represent a '0' (800ns)
    nop
    return                 ; Return from function


_write_1:
    inc LATA,           ; Set data pin high
    repeat #3          ; Adjust based on clock speed for correct timing (700ns)
    nop
    clr LATA          ; Set data pin low
    repeat #2            ; Shorter delay for low (600ns)
    nop
    return                 ; Return from function

; Function to wait 100 microseconds (Example of a simple delay)
_wait_100us:
    repeat #1593
    nop
    return
   

; Function to wait 1 millisecond (Example of a longer delay)
_wait_1ms:
   repeat #15993
   nop
   return




