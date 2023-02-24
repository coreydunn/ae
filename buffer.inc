; vi: ft=nasm

buffer_size equ 16777216
buffer_filen_size equ 4096
O_RDWR equ      0x2

section .bss
buffer: resb buffer_size
buffer_idx: resd 1
buffer_filen: resb buffer_filen_size

section .data

section .text
global buf_print,buf_cpy,buf_read
extern puts,printf,strcpy,read,open,memcpy
buf_print:
	mov rdi,buffer
	call puts
	ret

; buf_cpy(char)
buf_cpy:
	xor rax,rax
	mov eax,[buffer_idx]
	mov [buffer+rax],dil
	inc dword[buffer_idx]
	ret

; buf_read(char*filename)
buf_read:
	push rbp
	mov rbp,rsp
	sub rsp,12
%define fd    -4
%define filen -12

	;mov dword[rsp+fd],0
	mov [rsp+filen],rdi

	; copy filename to buffer_filen
	xor rcx,rcx
.l01:
	mov rdi,[rsp+filen]
	mov al,[rdi+rcx]
	cmp al,0
	jz .l01e
	mov [buffer_filen+rcx],al
	inc rcx
	jmp .l01
.l01e:

	; read file contents into buffer
	mov rdi,[rsp+filen]
	mov esi,O_RDWR
	call open
	mov [rsp+fd],eax
	cmp eax,-1
	jz .end

	; read file into buffer
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
