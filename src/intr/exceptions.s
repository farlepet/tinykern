.globl excep_df
.type excep_df, @function
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

df.err: .long 0


.globl excep_gpf
.type excep_gpf, @function
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

gpf.eip: .long 0
gpf.err: .long 0

.globl excep_pf
.type excep_pf, @function
excep_pf: # Page Fault
    push %eax
    mov 4(%esp), %eax
    mov %eax, (pf.err)
    mov 8(%esp), %eax
    mov %eax, (pf.eip)
    pop %eax

    pusha
    push %ebp
    mov %cr2, %eax
    push %eax     # cr2
    push (pf.err) # err code
    push (pf.eip) # code location
    call hand_pagefault
    add $16, %esp
    popa
    add $4, %esp
    iret

pf.eip: .long 0
pf.err: .long 0


.global excep_ts
.type excep_tf, @function
excep_ts: # Invalid TSS
    push %eax
    mov 4(%esp), %eax
    mov %eax, (ts.err)
    mov 8(%esp), %eax
    mov %eax, (ts.eip)
    pop %eax

    pusha
    push (ts.err)
    push (ts.eip)
    call hand_invl_tss
    add $8, %esp
    popa
    add $4, %esp
    iret

ts.err: .long 0
ts.eip: .long 0
