
.include "xc.inc"

.text  ; Directs the assembler to put the following data in ROM (program memory)

; Global statement for external visibility
.global  _wait_1ms
   

_wait_1ms:
    NOP                       ; No Operation (placeholder)
    RETURN                    ; Return from the subroutine


