#ifndef DISPLAY_BUFFER_H_
#define DISPLAY_BUFFER_H_

#include <SDL2/SDL.h>

#define DISPLAY_BUFFER_WIDTH 64
#define DISPLAY_BUFFER_HEIGHT 32

class Display_Buffer {

    public:

        Display_Buffer(SDL_Renderer* renderer);
        ~Display_Buffer();

        bool draw_sprite(unsigned char x, unsigned char y, unsigned char *sprite, unsigned char sprite_bytes);
        void display_contents();
        void print_contents();

    private:

        unsigned char contents[DISPLAY_BUFFER_WIDTH][DISPLAY_BUFFER_HEIGHT];

        SDL_Renderer* renderer;
        SDL_Texture* texture;
};

#endif

