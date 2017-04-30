#ifndef CHIP8_H_
#define CHIP8_H_

#include <stack>
#include "Display_Buffer.h"

#define NUM_V_REGISTERS 16
#define MEMORY_SIZE 4096

class Chip8 {

    public:

        Chip8(Display_Buffer* display_buffer);
        ~Chip8();

        bool load_program_from_file(char* filename);
        bool execute_one_instruction();

        void print_registers();
        void print_memory_contents();

    private:

        unsigned char v_reg[NUM_V_REGISTERS];
        unsigned short i_reg;
        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short pc;
        std::stack<unsigned short> stack;

        unsigned char memory[MEMORY_SIZE];
        Display_Buffer* display_buffer;

        bool debug;
};

#endif

