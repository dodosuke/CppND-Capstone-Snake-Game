#include "tetromino.h"
#include <memory>
#include <algorithm>
#include <iostream>

Tetromino::Tetromino(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height){
    };

void Tetromino::AddCandidates() {
    int x = (blocks.back())->location.x;
    int y = (blocks.back())->location.y;

    SDL_Point diffs [4] {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (SDL_Point diff : diffs){
        SDL_Point candidate {SDL_Point{x+diff.x, y+diff.y}};

        // Add the candidate if it is not overlapped with the blocks
        if (std::find_if(blocks.begin(), blocks.end(), compare(candidate)) == blocks.end()) {
            //std::cout << candidate.x << "+" << candidate.y << std::endl;
            candidates.emplace_back(candidate);
        }
    }
}

void Tetromino::GenerateBlocks() {
    SDL_Point initLoc {4, 4};
    int index {0};

    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;

    std::vector<Block*>{}.swap(blocks);

    while (blocks.size() < this->size) {

        if (blocks.size() == 0) {
            // Add initial block
            blocks.emplace_back(new Block(initLoc, r, g, b, a));
        } else {
            // Add a random block
            index = rand() % candidates.size();
            blocks.emplace_back(new Block(std::move(candidates[index]), r, g, b, a));
            candidates.erase(candidates.begin()+index);
        }

        AddCandidates();
        //std::cout<<candidates.size()<<std::endl;
    }

    std::vector<SDL_Point>{}.swap(candidates);

/*     for (Block* b : blocks) {
        std::cout << b->location.x << ":" << b->location.y << std::endl;
    } */
}

void Tetromino::Move(Direction direction) {
    switch (direction) {
        case Direction::Up:
            break;

        case Direction::Down:
            if (MovableToDown()) {
                for (Block* &b : blocks) {
                    b->location.y += 1;
                }
            } else {
                MoveToStack();
            }
            break;

        case Direction::Right:
            if (MovableToRight()) {
                for (Block* &b : blocks) {
                    b->location.x += 1;
                }
            }
            break;

        case Direction::Left:
            if (MovableToLeft()) {
                for (Block* &b : blocks) {
                    b->location.x -= 1;
                }
            }
            break;
    }
}

bool Tetromino::MovableToLeft() {
    for (Block* const &b : blocks) {
        if (b->location.x == 0) {
            return false;
        }

        for (Block* const &s : stack) {
            if (((b->location.x - 1) == s->location.x) && (b->location.y == s->location.y)) {
                return false;
            }
        }
    }

    return true;
}

bool Tetromino::MovableToRight() {
    for (Block* const &b : blocks) {
        if (b->location.x == (grid_width - 1)) {
            return false;
        }

        for (Block* const &s : stack) {
            if (((b->location.x + 1) == s->location.x) && (b->location.y == s->location.y)) {
                return false;
            }
        }
    }

    return true;
}

bool Tetromino::MovableToDown() {
    for (Block* const &b : blocks) {
        if (b->location.y == (grid_height - 1)) {
            return false;
        }

        for (Block* const &s : stack) {
            if (((b->location.y + 1) == s->location.y) && (b->location.x == s->location.x)) {
                return false;
            }
        }
    }

    return true;
}

void Tetromino::MoveToStack() {
    // TODO
    for (Block* b : blocks) {
        stack.emplace_back(std::move(b));
    }

    GenerateBlocks();
}

