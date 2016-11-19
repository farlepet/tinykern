.globl idt_lidt
.type idt_lidt, @function
idt_lidt:
    mov 4(%esp), %eax
    lidt (%eax)
    ret
