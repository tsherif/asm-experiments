%include "lib.inc"

section .data
message: db '-1234asdf', 0
output: db 'xxxxxxxx'

section .text
global _start

_start:
mov rdi, message
call parse_int
push rdx
mov rdi, rax
call print_int
call print_newline

mov rax, 60
pop rdi
syscall
