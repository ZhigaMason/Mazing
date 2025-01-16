#include <iostream>
#include "grid.hpp"

int main() {
        srand(12);
        for(int i = 0; i < 10; ++i) {
                Grid g(10, 10);
                g.fill_grid(rand());
                std::cout << g << std::endl;
        }
}
