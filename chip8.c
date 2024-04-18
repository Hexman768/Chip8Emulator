#include <stdio.h>
#include <string.h>
#include "chip8.h"

void init() {
    memcpy(memory, fontset, 80*sizeof(uint8_t));
    memset(stack, 0, 16);
    memset(graphics, 0, SC_WIDTH * SC_HEIGHT);
}

void mem_dump() {
    for (int i = 0; i < 4096; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%x", memory[i]);
        }
        printf("\n");
    }
}

int load_ROM(char *rom) {
    FILE* fp;
    fp = fopen(rom, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open ROM!\n");
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(memory + 0x200, sizeof(uint16_t), size, fp);
    fclose(fp);
    return 1;
}

void fetch(){
    return;
}

void decode(){
    return;
}

void execute(){
    return;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("[ERROR] Usage: ./chip8 <input ROM path>\n");
        return 0;
    }

    // initialize memory 
    init();

    // read ROM into chip-8 memory
    load_ROM(argv[1]);
    mem_dump();
    return 0;
}

