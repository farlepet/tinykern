.globl paging_enable
paging_enable:
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ret

.globl paging_disable
paging_disable:
    mov %cr0, %eax
    and $0x7FFFFFFF, %eax
    mov %eax, %cr0
    ret

.globl paging_set_dir
paging_set_dir:
    mov 4(%esp), %eax
    mov %eax, %cr3
    ret
