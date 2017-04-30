#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include "Chip8.h"

using namespace std;

#define PROGRAM_MEMORY_OFFSET 0x200

Chip8::Chip8(Display_Buffer* display_buffer) {
    for (unsigned int i = 0; i < NUM_V_REGISTERS; i++) {
        v_reg[i] = 0;
    }

    i_reg = 0;
    delay_timer = 0;
    sound_timer = 0;

    pc = PROGRAM_MEMORY_OFFSET;

    fill(memory, memory + MEMORY_SIZE, 0);

    this->display_buffer = display_buffer;

#ifdef DEBUG
    debug = true;
#else
    debug = false;
#endif
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

bool Chip8::execute_one_instruction() {
    unsigned short next_pc = pc + 2;

    unsigned short byte0 = memory[pc];
    unsigned short byte1 = memory[pc + 1];
    unsigned short nibble0 = byte0 >> 4;
    unsigned short nibble1 = byte0 & 0x0F;
    unsigned short nibble2 = byte1 >> 4;
    unsigned short nibble3 = byte1 & 0x0F;

    if (nibble0 == 0x1) {
        next_pc = (nibble1 << 8) | byte1;
        if (debug) fprintf(stderr, "DEBUG: Jumping to 0x%03X (setting PC to that).\n", next_pc);
        if (next_pc == pc) {
            fprintf(stderr, "WARNING: Stall has been detected - will halt further execution!\n");
            return false;
        }
    } else if (nibble0 == 0x3) {
        if (v_reg[nibble1] == byte1) {
            if (debug) fprintf(stderr, "DEBUG: V%X equals 0x%02X, so the next instruction will be skipped.\n", nibble1, byte1);
            next_pc += 2;
        } else {
            if (debug) fprintf(stderr, "DEBUG: V%X (0x%02X) does NOT equal 0x%02X, so the next instruction will NOT be skipped.\n",
                    nibble1, v_reg[nibble1], byte1);
        }
    } else if (nibble0 == 0x6) {
        if (debug) fprintf(stderr, "DEBUG: Setting V%X to 0x%02X.\n", nibble1, byte1);
        v_reg[nibble1] = byte1;
    } else if (nibble0 == 0x7) {
        if (debug) fprintf(stderr, "DEBUG: Adding 0x%02X to V%X (0x%02X) to get 0x%02X.\n", byte1, nibble1, v_reg[nibble1], v_reg[nibble1] + byte1);
        v_reg[nibble1] += byte1;
    } else if (nibble0 == 0xA) {
        unsigned short new_i_reg = (nibble1 << 8) | byte1;
        if (debug) fprintf(stderr, "DEBUG: Loading I register with: 0x%03X\n", new_i_reg);
        i_reg = new_i_reg;
    } else if (nibble0 == 0xC) {
        unsigned short random_byte = rand() % 256;
        if (debug) fprintf(stderr, "DEBUG: Random byte 0x%02X ANDed with 0x%02X to get 0x%02X, and that will be put into V%X.\n",
                random_byte, byte1, random_byte & byte1, nibble1);
        v_reg[nibble1] = random_byte & byte1;
    } else if (nibble0 == 0xD) {
        if (debug) fprintf(stderr, "DEBUG: Drawing a sprite of 0x%X bytes at (V%X, V%X)=(0x%02X, 0x%02X)=(%u, %u).\n",
                nibble3, nibble1, nibble2, v_reg[nibble1], v_reg[nibble2], v_reg[nibble1], v_reg[nibble2]);
        if (display_buffer->draw_sprite(v_reg[nibble1], v_reg[nibble2], &memory[i_reg], nibble3)) {
            v_reg[0xF] = 0x01;
        } else {
            v_reg[0xF] = 0x00;
        }
    } else {
        fprintf(stderr, "WARNING: Unimplemented instruction at 0x%04X: 0x%02X%02X - not taking any action!\n",
                pc, memory[pc], memory[pc + 1]);
    }

    pc = next_pc;

    return true;
}

void Chip8::print_registers() {
    for (int i = 0; i < 4; i++) {
        printf("V%X: 0x%02X    V%X: 0x%02X    V%X: 0x%02X    V%X: 0x%02X\n",
                i, v_reg[i],
                i + 4, v_reg[i + 4],
                i + 8, v_reg[i + 8],
                i + 12, v_reg[i + 12]);
    }

    printf("\nPC: 0x%03X    I: 0x%03X    DELAY: 0x%02X    SOUND: 0x%02X\n", pc, i_reg, delay_timer, sound_timer);
}

void Chip8::print_memory_contents() {
    printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int i = 0; i < MEMORY_SIZE; i += 16) {
        printf("0x%02X:", i / 16);

        for (int j = i; j < i + 16; j++) {
            printf(" %02X", memory[j]);
        }

        printf("\n");
    }
}

