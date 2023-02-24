; vi: ft=nasm

buffer_size       equ 16777216
buffer_filen_size equ 4096
O_RDONLY          equ 0x00
O_WRONLY          equ 0x01
O_RDWR            equ 0x02
O_CREAT           equ 0x40

section .bss
buffer: resb buffer_size
buffer_idx: resd 1
buffer_filen: resb buffer_filen_size

section .data
f01: db "writing '%s'",10,0
f02: db "error: failed opening file '%s'",10,0

section .text
global buf_print,buf_cpy,buf_read,buf_write
extern puts,printf,strcpy,read,open,memcpy,close,write
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
	mov esi,O_RDONLY
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

	cmp dword[rsp+fd],-1
	jz .end
	mov edi,[rsp+fd]
	call close

.end:
	pop rbp
	add rsp,12
	ret
%undef fd
%undef filen

; buf_write(char*filename)
buf_write:
	push rbp
	mov rbp,rsp
	sub rsp,64
%define fd -4
%define filen -12

	cmp rdi,0
	jz .end
	mov [rsp+filen],rdi

	lea rdi,[f01]
	mov rsi,[rsp+filen]
	call printf

	; open file
	lea rdi,[rsp+filen]
	mov rsi,O_WRONLY|O_CREAT
	mov edx,0x1a4
	call open
	mov [rsp+fd],eax
	cmp eax,-1
	jz .fail

	; write to file
	mov edi,[rsp+fd]
	;mov edi,1
	lea rsi,[buffer]
	mov rdx,64
	call write

	; close file
	mov edi,[rsp+fd]
	call close

	jmp .end
.fail:
	xor rax,rax
	lea rdi,[f02]
	lea rsi,[rsp+filen]
	call printf

.end:
	pop rbp
	add rsp,64
	ret
%undef fd
%undef filen
