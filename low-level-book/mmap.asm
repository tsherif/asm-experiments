%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

%include "lib.inc"

section .data
fname: db 'test.txt', 0

section .text
global _start

_start:
mov rax, 2      ;syscall open
mov rdi, fname
mov rsi, O_RDONLY
mov rdx, 0
syscall

mov r8, rax     ;rax = file descriptor, r8 (optional fd for mmap)
mov rax, 9      ;syscall mmap
mov rdi, 0      ;let OS choose address
mov rsi, 4096   ;page size
mov rdx, PROT_READ
mov r10, MAP_PRIVATE
mov r9, 0       ;offset into file
syscall

mov rdi, rax
call print_string

xor rdi, rdi
call exit

