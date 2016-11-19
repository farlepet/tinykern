.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Multiboot header
.section .mboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Kernel entrypoint
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    mov %esp, %ebp

    push %eax # Magic Number
    push %ebx # Multiboot Header pointer
    call __kernel_entry
    
# Kernel returned, loop forever:
    cli
h:  hlt
    jmp h

.size _start, . - _start
