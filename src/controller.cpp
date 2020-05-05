#include <iostream>
#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &running, Tetromino &tetromino) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          break;

        case SDLK_DOWN:
          tetromino.Move(Tetromino::Direction::Down);
          break;

        case SDLK_LEFT:
          tetromino.Move(Tetromino::Direction::Left);
          break;

        case SDLK_RIGHT:
          tetromino.Move(Tetromino::Direction::Right);
          break;

        case SDLK_a:
          running = false;
          break;
      }
    }
  }
}