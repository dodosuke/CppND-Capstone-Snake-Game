#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "block.h"

// Ref: https://www.techiedelight.com/check-vector-contains-given-element-cpp/
struct compare {
  SDL_Point loc;
  compare(SDL_Point const &l): loc{l} { }

  bool operator()(Block* const &b){
    return ((b->location.x == loc.x) && (b->location.y == loc.y));
  }
};

class Tetromino {
 public:
  enum class Direction {Up, Down, Left, Right};

  Tetromino(int grid_width, int grid_height);
  int r {rand() % 256};
  int g {rand() % 256};
  int b {rand() % 256};
  int a {255};
  std::vector<Block*> blocks {};
  std::vector<Block*> stack {};

  void GenerateBlocks();
  void Move(Direction direction);

 private:
  int size{4};
  int grid_width;
  int grid_height;

  std::vector<SDL_Point > candidates {};

  void AddCandidates();
  void MoveToStack();

  bool MovableToLeft();
  bool MovableToRight();
  bool MovableToDown();
};

#endif