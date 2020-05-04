#ifndef BLOCK_H
#define BLOCK_H

#include "SDL.h"

struct Block {
    SDL_Point location;
    int color [4];

    Block(SDL_Point loc, int r, int g, int b, int a)
    : location {loc},
      color{r, g, b, a}{};
};

#endif