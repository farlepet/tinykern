.globl idt_lidt
idt_lidt:
    mov 4(%esp), %eax
    lidt (%eax)
    ret
