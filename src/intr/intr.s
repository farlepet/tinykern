.globl intr_test_intr
.type intr_test_intr, @function
intr_test_intr:
    pusha
    push $str
    call kputs
    add $4, %esp
    popa
    iret

.globl intr_dummy_int
.type intr_dummy_int, @function
intr_dummy_int:
    cli
    hlt
    jmp intr_dummy_int


str:
    .string "Test Interrupt Fired!\n"
    .byte 0
