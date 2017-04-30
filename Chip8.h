#ifndef CHIP8_H_
#define CHIP8_H_

#define NUM_V_REGISTERS 16
#define MEMORY_SIZE 4096

class Chip8 {

    public:

        Chip8();
        ~Chip8();

        bool load_program_from_file(char* filename);
        void execute_instructions(int num_instructions);

        void print_registers();
        void print_memory_contents_all();

    private:

        unsigned char v_reg[NUM_V_REGISTERS];
        unsigned short i_reg;
        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned char memory[MEMORY_SIZE];
};

#endif

