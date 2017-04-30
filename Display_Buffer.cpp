#include <stdio.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include "Display_Buffer.h"

using namespace std;

#define PIXEL_ON 0X1
#define PIXEL_OFF 0X0

Display_Buffer::Display_Buffer(SDL_Renderer* renderer) {
    this->renderer = renderer;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);

    fill((unsigned char *)contents, (unsigned char *)contents + (DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_HEIGHT), 0);
}

Display_Buffer::~Display_Buffer() {
    // nothing special to do here at the moment
}

bool Display_Buffer::draw_sprite(unsigned char x, unsigned char y, unsigned char *sprite, unsigned char sprite_bytes) {
    bool collision_detected = false;

    for (unsigned char byte = 0; byte < sprite_bytes; byte++, y++) {
        for (unsigned char bit = 0; bit < 8; bit++) {
            unsigned char effective_x = (x + bit) % DISPLAY_BUFFER_WIDTH;
            unsigned char effective_y = y % DISPLAY_BUFFER_HEIGHT;

            unsigned char contents_value = contents[effective_x][effective_y];
            unsigned char sprite_value = (sprite[byte] & (0x1 << (7 - bit))) ? PIXEL_ON : PIXEL_OFF;

            if (contents_value == PIXEL_ON && sprite_value == PIXEL_OFF) {
                collision_detected = true;
            }

            contents[effective_x][effective_y] = contents_value ^ sprite_value;
        }
    }

    return collision_detected;
}

void Display_Buffer::display_contents() {
    unsigned int pixels[DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_HEIGHT];
    for (int y = 0; y < DISPLAY_BUFFER_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_BUFFER_WIDTH; x++) {
            pixels[DISPLAY_BUFFER_WIDTH * y + x] = contents[x][y] ? 0xFFFFFFFF : 0x00000000;
        }
    }

    SDL_UpdateTexture(texture, NULL, pixels, 4 * DISPLAY_BUFFER_WIDTH);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Display_Buffer::print_contents() {
    printf("================================================================\n");
    for (int y = 0; y < DISPLAY_BUFFER_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_BUFFER_WIDTH; x++) {
            printf(contents[x][y] ? "*" : " ");
        }
        printf("\n");
    }
    printf("================================================================\n");
}

