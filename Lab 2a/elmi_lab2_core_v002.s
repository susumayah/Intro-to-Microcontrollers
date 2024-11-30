.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

    .bss        ; put the following labels in RAM
counter:
    .space 2    ; a variable that takes two bytes (we won?t use
                ; it for now, but put here to make this a generic
                ; template to be used later).
stack:
    .space  32  ; this will be our stack area, needed for func calls

.text           ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _main               ;BP
;your functions go here

write_0:
    inc LATA          ; Set data pin high (for example RA0)
    nop
    nop
    clr LATA          ; Set data pin low
    repeat #2          ; Longer delay to represent a '0' (800ns)
    nop
    return                 ; Return from function

; Write 1 function (700ns High, 600ns Low)
write_1:
    inc LATA,           ; Set data pin high
    repeat #3          ; Adjust based on clock speed for correct timing (700ns)
    nop
    clr LATA          ; Set data pin low
    repeat #2            ; Shorter delay for low (600ns)
    nop
    return                 ; Return from function        



; Color function for sending red (GRB format)
write_color_red:
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    return

; Color function for sending green
write_color_green:
    ; Green 8 bits
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0

    return

; Color function for sending blue 
write_color_blue:
 
    ; Blue 8 bits
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    return
    
delay_1sec:
    mov #62500, w0            ; Approximate count for 1-second delay
delay_loop:
    sub w0, #1, w0            ; Decrement counter
    bra nz, delay_loop        ; Loop until zero
    return                    ; Return from delay subroutine
_main:
    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0000,w0  
    mov     w0,LATA            ; set pin RA0 high

foreverLoop:
    call write_color_red
    call write_color_green
    call write_color_blue
    call delay_1sec            ; Wait for 1 second
    bra  foreverLoop
    nop

.end