.globl irq_pit_hand
.type irq_pit_hand, @function
irq_pit_hand:
    pusha

    call pit_handler

    popa
    iret
