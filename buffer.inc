section .data
fmt: db '%s',0
section .text
global print,cpy,rd
extern puts,printf,strcpy,read,open
print:
	mov rdi,buffer
	call puts
	ret

; cpy(char)
cpy:
	xor rax,rax
	mov eax,[buffer_idx]
	mov [buffer+rax],dil
	inc dword[buffer_idx]
	ret

; rd(char*filename)
rd:
	push rbp
	mov rbp,rsp
	sub rsp,12
%define fd    -4
%define filen -12
%define O_RDWR 0x2

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
%undef O_RDWR
