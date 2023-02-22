; vi: ft=nasm

buffer_size equ 16777216
O_RDWR equ      0x2

section .bss
buffer: resb buffer_size
buffer_idx: resd 1

section .data

section .text
global buf_print,buf_cpy,buf_read
extern puts,printf,strcpy,read,open
buf_print:
	mov rdi,buffer
	call puts
	ret

; cpy(char)
buf_cpy:
	xor rax,rax
	mov eax,[buffer_idx]
	mov [buffer+rax],dil
	inc dword[buffer_idx]
	ret

; rd(char*filename)
buf_read:
	push rbp
	mov rbp,rsp
	sub rsp,12
%define fd    -4
%define filen -12

	;mov dword[rsp+fd],0
	mov [rsp+filen],rdi

	mov rdi,[rsp+filen]
	mov esi,O_RDWR
	call open
	mov [rsp+fd],eax
	cmp eax,-1
	jz .end

	mov edi,[rsp+fd]
	mov rsi,buffer
	mov edx,buffer_size
	call read
	mov [buffer_idx],eax

.end:
	pop rbp
	add rsp,12
	ret
%undef fd
%undef filen
