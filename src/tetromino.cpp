#include "tetromino.h"
#include <memory>
#include <algorithm>
#include <iostream>

Line::Line(Block* block)
    : index(block->location.y),
      blocks{std::move(block)}{}

Line::~Line(){
    // delete all blocks
    for (auto it = std::begin(blocks); it != std::end(blocks); ++it){ delete *it; }
}

void Line::Add(Block* block) {
    if (block->location.y == index) {
        blocks.emplace_back(block);
    }
}

void Line::Down() {
    index += 1;
    for (auto b : blocks) {
        b->location.y += 1;
    }
}

Tetromino::Tetromino(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height){
        GenerateBlocks();
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

        for (auto const &l : stack) {
            for (auto const &s : l->blocks) {
                if (((b->location.x - 1) == s->location.x) && (b->location.y == s->location.y)) {
                    return false;
                }
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

        for (auto const &l : stack) {
            for (auto const &s : l->blocks) {
                if (((b->location.x + 1) == s->location.x) && (b->location.y == s->location.y))  {
                    return false;
                }
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

        for (auto const &l : stack) {
            for (auto const &s : l->blocks) {
                if (((b->location.y + 1) == s->location.y) && (b->location.x == s->location.x))  {
                    return false;
                }
            }
        }
    }

    return true;
}

void Tetromino::MoveToStack() {
    // TODO
    for (Block* b : blocks) {
        bool isNew {true};
        for (auto const &l : stack) {
            if (b->location.y == l->index) {
                isNew = false;
                l->Add(b);
                break;
            }
        }
        if (isNew) {
            stack.emplace_back(new Line(b));
        }
    }

    for (auto it = stack.begin(); it != stack.end();) {
        if ((*it)->blocks.size() == grid_width) {
            std::cout<<"yay"<<std::endl;
            DeleteLine((*it)->index);
        } else {
            it++;
        }

    }

    GenerateBlocks();
}

void Tetromino::DeleteLine(int index) {
    for (auto it = stack.begin(); it != stack.end();) {
        if ((*it)->index == index) {
            delete *it;
            stack.erase(it);
        } else if ((*it)->index < index) {\
            (*it)->Down();
            it++;
        } else {
            it++;
        }
    }
}
