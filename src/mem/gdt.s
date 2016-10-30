.globl gdt_lgdt
gdt_lgdt:
    mov 4(%esp), %eax
    lgdt (%eax)
    ret

.global gdt_seg_reload
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
