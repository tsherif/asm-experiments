section .data
newline: db 10
digits: db "0123456789"

section .text

string_length:
    xor rax, rax
.loop:
    cmp byte[rdi + rax], 0
    je .end
    inc rax
    jmp .loop

.end:
    ret

print_string:
    call string_length
    mov rdx, rax
    mov rax, 1
    mov rsi, rdi
    mov rdi, 1
    syscall
    ret


print_char:
    push rdi
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rdi
    ret

print_newline:
    mov rax, 1
    mov rsi, newline
    mov rdi, 1
    mov rdx, 1
    syscall
    ret


print_uint:
    push 0
    mov rcx, 10
    mov r9, 10
    mov r10, 0
    mov rax, rdi
.loop:
    xor rdx, rdx ;IMPORTANT div uses RDX:RAX as dividend!
    div rcx
    mov r10b, byte[digits + rdx]
    push r10
    cmp rax, 0
    je .print
    jmp .loop
.print:
    pop rdi
    cmp rdi, 0
    je .end
    call print_char
    jmp .print
.end:
    ret


print_int:
    cmp rdi, 0
    jge .print_digits
    push rdi
    mov rdi, 45
    call print_char
    pop rdi
    neg rdi
.print_digits:
    call print_uint
    ret

string_equals:
    xor rax, rax
.loop:
    mov dl, [rsi+rax]
    cmp [rdi+rax], dl 
    jne .false
    cmp byte[rdi+rax], 0
    je  .true
    inc rax
    jmp .loop
.true:
    mov rax, 1
    jmp .end
.false:
    mov rax, 0
.end:
    ret


read_char:
    push 0
    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rax
    ret 

read_word:
    push rdi        ;output buffer
    push rsi        ;buffer length
    call read_char
    pop rsi
    pop rdi
    cmp rax, 0x20
    je read_word
    cmp rax, 0x9
    je read_word
    cmp rax, 0xA
    je read_word
    xor rcx, rcx    ;current index
    dec rsi         ;leave space for null
.loop:
    cmp rcx, rsi
    je .overflow
    mov [rdi + rcx], al
    inc rcx
    push rdi
    push rsi
    push rcx
    call read_char
    pop rcx
    pop rsi
    pop rdi
    cmp rax, 0x20
    je .null_terminate
    cmp rax, 0x9
    je .null_terminate
    cmp rax, 0xA
    je .null_terminate
    jmp .loop
.null_terminate:
    mov byte[rdi + rcx], 0
    mov rax, rdi
    jmp .end
.overflow:
    xor rax, rax
.end:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rdx, rdx
.length_loop:
    cmp byte[rdi + rdx], "0"
    jl .parse
    cmp byte[rdi + rdx], "9"
    jg .parse
    inc rdx
    jmp .length_loop
.parse:
    mov rcx, rdx
    push rdx ;mul modifies rdx!!!!
    dec rcx
    xor rsi, rsi
    mov r9, 1
    mov r10, 10
.loop:
    cmp rcx, 0
    jl  .end
    xor rax, rax
    mov al, byte[rdi + rcx]
    sub rax, "0"
    mul r9
    add rsi, rax
    mov rax, r9
    mul r10
    mov r9, rax
    dec rcx
    jmp .loop
.end:
    mov rax, rsi
    pop rdx
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rcx, rcx
    cmp byte[rdi], "-"
    jne .digits
    inc rdi
    mov rcx, 1
.digits:
    push rcx
    call parse_uint
    pop rcx
    cmp rcx, 1
    jne .end
    neg rax
    inc rdx
.end:
    ret 


string_copy:
    xor rcx, rcx
.loop:
    cmp rdx, 0
    je .overflow
    mov r9b, [rdi + rcx]
    cmp r9b, 0
    je .null_byte
    mov [rsi + rcx], r9b
    inc rcx
    dec rdx
    jmp .loop
.overflow:
    mov rax, 0
    jmp .end 
.null_byte:
    mov byte[rsi + rcx], 0
    mov rax, rsi
.end:
    ret
