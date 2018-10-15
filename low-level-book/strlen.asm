global _start

section .data
test_string: db "abcdef", 0

section .text

strlen:
    pop rcx
    xor rax, rax

.loop:
    cmp byte[rsi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    push rax
    push rcx
    ret

_start:
    mov rsi, test_string
    call strlen
    pop rdi

    mov rax, 60
    syscall
