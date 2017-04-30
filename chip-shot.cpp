#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include "Chip8.h"
#include "Display_Buffer.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s file_to_load\n", argv[0]);
        return 1;
    }
    char* filename = argv[1];

    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(filename, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 256, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Display_Buffer display_buffer(renderer);

    Chip8 chip8(&display_buffer);
    if (chip8.load_program_from_file(filename)) {

        while (chip8.execute_one_instruction())
            ;

        chip8.print_memory_contents();
        printf("\n");
        chip8.print_registers();
        display_buffer.print_contents();
        display_buffer.display_contents();
    } else {
        fprintf(stderr, "ERROR: Couldn't load program from file \"%s\"!\n", filename);
        return EXIT_FAILURE;
    }

    bool done = false;
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                done = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                done = true;
            } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                display_buffer.display_contents();
            }
        }
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

