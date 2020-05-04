#include "tetromino.h"
#include <memory>
#include <algorithm>
#include <iostream>

Tetromino::Tetromino(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height){
    };

void Tetromino::AddCandidates() {
    int x = blocks.back().location.x;
    int y = blocks.back().location.y;

    SDL_Point diffs [4] {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (SDL_Point diff : diffs){
        SDL_Point candidate {SDL_Point{x+diff.x, y+diff.y}};

        if (std::find_if(blocks.begin(), blocks.end(), compare(candidate)) == blocks.end()) {
            candidates.emplace_back(candidate);
        }
    }
}

void Tetromino::GenerateBlocks() {
    SDL_Point initLoc {4, 4};
    int index {0};

    std::vector<Block>{}.swap(blocks);

    while (blocks.size() < this->size) {

        if (blocks.size() == 0) {
            // Add initial block
            blocks.emplace_back(Block(initLoc, r, g, b, a));
        } else {
            // Add a random block
            index = rand() % candidates.size();
            blocks.emplace_back(Block(candidates[index], r, g, b, a));
            candidates.erase(candidates.begin()+index);
        }

        AddCandidates();
    }

    std::vector<SDL_Point>{}.swap(candidates);
}


