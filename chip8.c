#include <stdio.h>
#include <string.h>
#include "chip8.h"

void init() {
    pc = 0x202;
    memcpy(memory, fontset, 80*sizeof(uint8_t));
    memset(stack, 0, 16);
    memset(screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void clear_screen() {
    memset(screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void mem_dump() {
    printf("\nAddr- ------------------Memory Dump------------------ ------------Symbol-------------\n");
    uint16_t addr = 0x200;
    for (int i = 0; i < 16; i++) {
        uint16_t ascii = addr;
        printf("%#x ", addr);
        // print hex values
        for (int j = 0; j < 16; j++) {
            if (memory[addr] <= 0xF) {
                printf("0");
            }
            printf("%#x ", memory[addr]);
            addr++;
        }
        for (int j = 0; j < 16; j++) {
            uint8_t val = memory[ascii];
            if (val >= 32 && val <= 126) {
                printf("%c ", memory[ascii]);
            } else {
                printf(". ");
            }
            ascii++;
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

void fetch() {
    // get opcode at current program counter position in memory
    opcode = memory[pc] << 8 | memory[pc + 1];
    printf("opcode: 0x%x\n", opcode);
    pc += 2;
}

void draw() {
    // TODO: Write code to draw to screen
    if (draw_flag == 1) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            for (int j = 0; j < SCREEN_WIDTH; j++) {
                printf("%d", screen[i * SCREEN_WIDTH + j]);
            }
            printf("\n");
        }
    }
    return;
}

void decode() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = (opcode & 0x000F);
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;
    printf("X: 0x%x, Y: 0x%x, n: 0x%x, nn: 0x%x, nnn: 0x%x\n", x, y, n, nn, nnn);
    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                printf("clear screen");
                clear_screen();
            }
            break;
        case 0x1000:
            pc = nnn;
            break;
        case 0x6000:
            v[x] = nn;
            // set register vx
            break;
        case 0x7000:
            // add value nn to vx
            v[x] += nn;
            break;
        case 0xA000:
            // set index register to NNN
            ir = nnn;
            break;
        case 0xD000:
            // draw instruction
            uint16_t x_pos = v[x];
            uint16_t y_pos = v[y];
            uint16_t height = n;
            uint8_t pixel;

            v[0xF] = 0;
            for (int i = 0; i < height; i++) {
                pixel = memory[ir + i];
                for (int j = 0; j < 8; j++) {
                    if ((pixel & (0x80 >> j)) != 0) {
                        if (screen[(x_pos + j + ((y_pos + i) * 64))] == 1) {
                            v[0xF] = 1;
                        }
                        screen[x_pos + j + ((y_pos + i) * 64)] ^= 1;
                    }
                }
            }
            draw_flag = 1;
            break;
    }
}

void execute() {
    return;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        //printf("[ERROR] Usage: ./chip8 <input ROM path>\n");
        fprintf(stderr, "[ERROR] Usage: ./chip8 <input ROM path>\n");
        return 1;
    }

    // initialize memory 
    init();

    // read ROM into chip-8 memory
    load_ROM(argv[1]);
    //mem_dump();

    int count = 0; 
    while (count < 20) {
        fetch();
        decode();
        execute();
        draw();
        count++;
    }
    //opcode = memory[pc] << 8 | memory[pc + 1];
    //decode(memory[pc], memory[pc + 1]);
    //mem_dump();
    return 0;
}

