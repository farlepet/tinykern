.globl excep_df
excep_df: # Double Fault
    push %eax
    mov 4(%esp), %eax
    mov %eax, (df.err)
    pop %eax

    pusha
    push (df.err) # err code
    call hand_doublefault
    add $4, %esp
    popa
    add $4, %esp
    iret

df.err:
    .long 0


.globl excep_gpf
excep_gpf: # General Protection Fault
    push %eax
    mov 4(%esp), %eax
    mov %eax, (gpf.err)
    mov 8(%esp), %eax
    mov %eax, (gpf.eip)
    pop %eax

    pusha
    push (gpf.err) # err code
    push (gpf.eip) # code location
    call hand_gpf
    add $8, %esp
    popa
    add $4, %esp
    iret

gpf.eip:
    .long 0
gpf.err:
    .long 0

.globl excep_pf
excep_pf: # Page Fault
    push %eax
    mov 4(%esp), %eax
    mov %eax, (pf.err)
    mov 8(%esp), %eax
    mov %eax, (pf.eip)
    pop %eax

    pusha
    mov %cr2, %eax
    push %eax     # cr2
    push (pf.err) # err code
    push (pf.eip) # code location
    call hand_pagefault
    add $12, %esp
    popa
    add $4, %esp
    iret

pf.eip:
    .long 0
pf.err:
    .long 0
