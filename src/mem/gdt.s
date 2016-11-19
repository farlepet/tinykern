.globl gdt_lgdt
.type gdt_lgdt, @function
gdt_lgdt:
    push %ebp
    mov  %esp,%ebp

    mov 8(%esp), %eax
    lgdt (%eax)

    pop %ebp
    ret

.globl gdt_seg_reload
.type gdt_seg_reload, @function
gdt_seg_reload:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $gdt_seg_reload.flush
gdt_seg_reload.flush:
    ret

.globl gdt_load_tss
.type gdt_load_tss, @function
gdt_load_tss:
    push %ebp
    mov %esp, %ebp

    mov $0x18, %ax
    ltr %ax
    
    pop %ebp
    ret

.globl gdt_set_tss
.type gdt_set_tss, @function
gdt_set_tss:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %ax
    ltr %ax
    pop %ebp
    ret

