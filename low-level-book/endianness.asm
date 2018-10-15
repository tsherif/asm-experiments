section .data
newline_char: db 10
codes: db "0123456789ABCDEF"
demo1: dq 0x1122334455667788
demo2: db 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88

section .text
global _start

print_newline:
    mov rax, 1              ;write syscall
    mov rdi, 1              ;stdout (rdi = destination index)
    mov rsi, newline_char   ;string address (rsi = source index)
    mov rdx, 1              ;number of characters to print (rdx = i/o data)
    syscall
    ret

print_hex:
    mov rax, rsi            ;rsi holds the number to print

    mov rdi, 1              ;stdout for the write syscalls
    mov rdx, 1              ;number of chars to print for write syscalls
    mov rcx, 64             ;number of bits in input number (rcx = cycle counter)
iterate:
    push rax                ;save original value of input number
    sub rcx, 4              ;rcx contains number of bits we're shifting this iteration
    sar rax, cl             ;shift input by value in rcx (cl = lowest byte in rcx, why is this necessary?)
    and rax, 0xF            ;mask lowest 4 bits (i.e. 1 hex digit)
    lea rsi, [codes + rax]  ;load address of ascii char for masked bits

    mov rax, 1              ;write syscall

    push rcx                ;syscall modifies rcx
    syscall
    pop rcx

    pop rax                 ;restore original input value in rax
    test rcx, rcx           ;check if bit count is at 0
    jnz iterate             ;if not iterate
    ret

exit:
    mov rax, 60             ;exit syscall code
    xor rdi, rdi            ;0 out rdi
    syscall

_start:
mov rsi, [demo1]
call print_hex
call print_newline

mov rsi, [demo2]
call print_hex
call print_newline

call exit
