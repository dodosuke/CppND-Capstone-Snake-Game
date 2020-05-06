#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <memory>
#include "SDL.h"

struct Block {
    SDL_Point location;
    int color [4];

    Block(SDL_Point loc, int r, int g, int b, int a)
    : location {loc},
      color{r, g, b, a}{};
};

// Ref: https://www.techiedelight.com/check-vector-contains-given-element-cpp/
struct compare {
  SDL_Point loc;
  compare(SDL_Point const &l): loc{l} { }

  bool operator()(Block* const &b){
    return ((b->location.x == loc.x) && (b->location.y == loc.y));
  }
};

class Line {
  public:
    int index;
    std::vector<Block*> blocks;

    Line(Block* block);
    ~Line();

    void Add(Block* block);
    void Down();
};

class Tetromino {
 public:
  enum class Direction {Up, Down, Left, Right};

  Tetromino(int grid_width, int grid_height);
  int r {rand() % 256};
  int g {rand() % 256};
  int b {rand() % 256};
  int a {255};

  int score {0};
  std::vector<Block*> blocks {};
  std::vector<Line*> stack {};

  void GenerateBlocks();
  void Move(Direction direction);

 private:
  int size{4};
  int grid_width;
  int grid_height;

  std::vector<SDL_Point > candidates {};

  void AddCandidates();
  void MoveToStack();
  void DeleteLine(int index);
  void Rotate();

  bool Rotatable();
  bool MovableToLeft();
  bool MovableToRight();
  bool MovableToDown();
};

#endif