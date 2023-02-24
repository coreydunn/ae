; vi: ft=nasm

%include 'buffer.inc'

section .data

section .text
global main
extern exit,buf_print,buf_cpy,buf_read,printf
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
	call buf_read
.L01:

	call buf_print

	xor edi,edi
	call exit
%undef argv
%undef argc
