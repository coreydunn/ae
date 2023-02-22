CC=       cc
CFLAGS=   -Wfatal-errors -g
LDFLAGS=   -no-pie

all: ae
main.o: main.asm buffer.inc
	nasm -f elf64 $<
%.o: %.asm
	nasm $^ -felf64
ae: main.o buffer.inc
	$(CC) $< -o $@ $(LDFLAGS)
clean:
	$(RM) *.o ae
