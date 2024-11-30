
generate_pulse:
    ; High pulse (24 cycles)
    repeat #22       ; 1 cycle to load and prep
    nop              ; 22+1 cycles to execute NOP 23 times
    inc    LATA      ; set pin RA0 high = 1 cycle
    ; Low pulse (32 cycles)
    repeat #30       ; 1 cycle to load and prep
    nop              ; 30+1 cycles to execute NOP 31 times
    clr    LATA      ; set pin RA0 low = 1 cycle
    return           ; 3 cycles for the return

; Function to provide a 100us delay
delay_100us:
    repeat #99       ; 1 cycle to load and prep
    nop              ; 99+1 cycles to execute NOP 100 times
    return           ; 3 cycles for the return

; Function to provide a 1ms delay
delay_1ms:
    repeat #999      ; 1 cycle to load and prep
    nop              ; 999+1 cycles to execute NOP 1000 times
    return           ; 3 cycles for the return

foreverLoop:
    call    generate_pulse    ; Call the pulse generation function
    bra     foreverLoop       ; Infinite loop



