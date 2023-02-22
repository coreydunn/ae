%include 'buffer.inc'
buffer_size equ 16777216
section .bss
buffer: resb buffer_size
buffer_idx: resd 1

section .data
f01: db '%d',10,0
str01: db '/tmp/file.txt',0

section .text
global main
extern exit,print,cpy,rd,printf
main:
	push rbp
	mov rbp,rsp
	sub rsp,64
%define argv -8
%define argc -12

	mov [rsp+argc],edi
	mov [rsp+argv],rsi

	; attempt to read file into buffer
	mov eax,[rsp+argc]
	cmp eax,2
	jl .L01
	mov rdi,[rsp+argv]
	mov rdi,[rdi+8]
	call rd
.L01:

	mov dil,'E'
	call cpy
	mov dil,'N'
	call cpy
	mov dil,'D'
	call cpy

	; print buffer
	mov rdi,str01
	call print

	mov edi,55
	call exit
%undef argv
%undef argc
