#include <iostream>
#include <vector>
#include "tiles.hpp"

int main() {
        std::vector tmp{ TILE::RIGHT, TILE::LEFTRIGHT, TILE::LEFTUP };
        std::cout << line_to_string(
               tmp.data(), tmp.size()
        );
}
