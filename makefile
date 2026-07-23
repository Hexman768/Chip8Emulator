all: chip8.c
		gcc -o c8 chip8.c

clean:
		rm -f c8
