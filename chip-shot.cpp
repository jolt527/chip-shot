#include <stdio.h>
#include <cstdlib>
#include "Chip8.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s file_to_load\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];

    Chip8 chip8;
    if (chip8.load_program_from_file(filename)) {
        chip8.print_registers();
        printf("\n");
        chip8.print_memory_contents_all();
    } else {
        fprintf(stderr, "ERROR: Couldn't load program from file \"%s\"!\n", filename);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

