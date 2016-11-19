.globl paging_enable
.type paging_enable, @function
paging_enable:
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ret

.globl paging_disable
.type paging_disable, @function
paging_disable:
    mov %cr0, %eax
    and $0x7FFFFFFF, %eax
    mov %eax, %cr0
    ret

.globl paging_set_dir
.type paging_set_dir, @function
paging_set_dir:
    mov 4(%esp), %eax
    mov %eax, %cr3
    ret

.globl paging_get_cr3
.type paging_get_cr3, @function
paging_get_cr3:
    mov %cr3, %eax
    ret
