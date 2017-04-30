#include <stdio.h>
#include <algorithm>
#include <fstream>
#include "Chip8.h"

using namespace std;

#define PROGRAM_MEMORY_OFFSET 0x200

Chip8::Chip8() {
    for (unsigned int i = 0; i < NUM_V_REGISTERS; i++) {
        v_reg[i] = 0;
    }

    i_reg = 0;
    delay_timer = 0;
    sound_timer = 0;

    fill(memory, memory + MEMORY_SIZE, 0);
}

Chip8::~Chip8() {
    // nothing special to do here at the moment
}

bool Chip8::load_program_from_file(char* filename) {
    ifstream file(filename, ios::in | ios::binary | ios::ate);
    if (!file.is_open()) {
        fprintf(stderr, "ERROR: Unable to open file \"%s\"!  (Possibly doesn't exist?)\n", filename);
        return false;
    }

    streampos file_size = file.tellg();
    if (file_size > MEMORY_SIZE - PROGRAM_MEMORY_OFFSET) {
        fprintf(stderr, "ERROR: Not enough free memory to load file \"%s\"! (%d file bytes > %d free bytes, %d bytes too many)\n",
                filename, (int)file_size, MEMORY_SIZE - PROGRAM_MEMORY_OFFSET, (int)file_size - (MEMORY_SIZE - PROGRAM_MEMORY_OFFSET));
        return false;
    }

    file.seekg(0, ios::beg);
    file.read((char*)(memory + PROGRAM_MEMORY_OFFSET), file_size);
    file.close();

    return true;
}

void Chip8::execute_instructions(int num_instructions) {
    //TODO
}

void Chip8::print_registers() {
    for (int i = 0; i < 4; i++) {
        printf("V%X: 0x%02X    V%X: 0x%02X    V%X: 0x%02X    V%X: 0x%02X\n",
                i, v_reg[i],
                i + 4, v_reg[i + 4],
                i + 8, v_reg[i + 8],
                i + 12, v_reg[i + 12]);
    }

    printf("\nI: 0x%03X    DELAY: 0x%02X    SOUND: 0x%02X\n", i_reg, delay_timer, sound_timer);
}

void Chip8::print_memory_contents_all() {
    printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int i = 0; i < MEMORY_SIZE; i += 16) {
        printf("0x%02X:", i / 16);

        for (int j = i; j < i + 16; j++) {
            printf(" %02X", memory[j]);
        }

        printf("\n");
    }
}

