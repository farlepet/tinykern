.globl irq_pit_hand
irq_pit_hand:
    pusha
    call pit_handler 
    popa
    iret
