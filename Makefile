CC=       cc
CFLAGS=   -Wfatal-errors -g
LDFLAGS=  -no-pie
ASMFLAGS= -felf64 -F dwarf -g

all: ae
main.o: main.asm buffer.inc
	nasm $(ASMFLAGS) $<
%.o: %.asm
	nasm $^ -felf64
ae: main.o buffer.inc
	$(CC) $< -o $@ $(LDFLAGS)
clean:
	$(RM) *.o ae
