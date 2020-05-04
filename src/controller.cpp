#include <iostream>
#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &running, Tetromino &tetromino) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          running = false;
          break;

        case SDLK_DOWN:
          tetromino.GenerateBlocks();
          break;

        case SDLK_LEFT:
          break;

        case SDLK_RIGHT:
          break;
      }
    }
  }
}