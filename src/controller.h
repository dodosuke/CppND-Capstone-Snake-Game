#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tetromino.h"

class Controller {
 public:
  void HandleInput(bool &running, Tetromino &tetromino) const;
};

#endif