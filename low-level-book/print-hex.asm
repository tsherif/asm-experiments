section .data
codes: db '0123456789ABCDEF'
newline: db 10

section .text
global _start

_start:
    mov rax, 0x1122334455667788
    mov rdi, 1 ;stdout
    mov rdx, 1 ;string length
    mov rbx, 64 ;number of bits in number to convert

.loop:
    push rax
    sub rbx, 4
    sar rax, bl
    and rax, 0xf

    lea rsi, [codes + rax]
    
    mov rax, 1 ;write opcode
    ; push rcx ;syscall changes rcx
    syscall ;write syscall
    ; pop rcx

    pop rax

    test rbx, rbx ;and rcx with itself (test for zero)
    jnz .loop

    ;print newline
    mov rax, 1 ;write opcode
    mov rsi, newline
    syscall

    mov rax, 60 ;exit opcode
    xor rdi, rdi ;zero out rdi (for return code)
    syscall
