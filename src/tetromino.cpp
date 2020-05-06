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
        if (IsCandidateAvailable(candidate)) {
            candidates.emplace_back(candidate);
        }
    }
}

bool Tetromino::IsCandidateAvailable(SDL_Point &point) {
    if (std::find_if(blocks.begin(), blocks.end(), compare(point)) != blocks.end()) {
        return false;
    }

    for (auto const &l : stack) {
        if (std::find_if(l->blocks.begin(), l->blocks.end(), compare(point)) != l->blocks.end()) {
            return false;
        }
    }

    return true;
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

        if (candidates.size() == 0) {
            running = false;
            break;
        }
    }

    std::vector<SDL_Point>{}.swap(candidates);

/*     for (Block* b : blocks) {
        std::cout << b->location.x << ":" << b->location.y << std::endl;
    } */
}

void Tetromino::Move(Direction direction) {
    switch (direction) {
        case Direction::Up:
            Rotate();
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

void Tetromino::Rotate() {
    if (!Rotatable()) {
        return;
    }

    for (int i {1}; i < size; i++) {
        int x = blocks[0]->location.x - blocks[i]->location.y + blocks[0]->location.y;
        int y = blocks[0]->location.y + blocks[i]->location.x - blocks[0]->location.x;

        blocks[i]->location.x = x;
        blocks[i]->location.y = y;
    }
}

bool Tetromino::Rotatable() {

    for (int i {1}; i < size; i++) {
        int x = blocks[0]->location.x - blocks[i]->location.y + blocks[0]->location.y;
        int y = blocks[0]->location.y + blocks[i]->location.x - blocks[0]->location.x;

        if ((x < 0) || (x >= grid_width)) {
            return false;
        }

        if ((y < 0) || (y >= grid_height)) {
            return false;
        }

        for (auto const &l : stack) {
            for (auto const &s : l->blocks) {
                if ((x == s->location.x) && (y == s->location.y)) {
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
            score += 1;
            //std::cout<<"yay"<<std::endl;
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
