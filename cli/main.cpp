#include <iostream>
#include "../src/grid/grid.hpp"

int main(int argc, char ** argv) {
        long seed = 0;
        if(argc == 2){
                seed = std::stol(argv[1], nullptr, 16);
                std::cerr << "Using seed: " << std::hex << seed << std::endl;
        }
        else std::cerr << "Using default seed" << std::endl;

        std::cerr << "Do you want to 10x10 grid with start and exit in corners? (Y/n)" << std::endl;

        std::string buf = "y";
        std::getline(std::cin, buf);
        if(buf != "n") {
                Grid g(10, 10);
                g.fill_grid(seed);
                std::cout << g;
                return 0;
        }

        int h, w, sx, sy, ex, ey;
        std::cerr << "Enter space-separated height and width of desired grid: " << std::flush;
        std::cin >> h >> w;
        std::cerr << "Enter space-separated x and y of start: " << std::flush;
        std::cin >> sx >> sy;
        std::cerr << "Enter space-separated x and y of exit: " << std::flush;
        std::cin >> ex >> ey;
        if(!std::cin) {
                std::cerr << "Invalid argument for one of the variables. Stoping." << std::endl;
                return 1;
        }
        Grid g(h, w, Grid::Coords(sx, sy), Grid::Coords(ex, ey));
        g.fill_grid(seed);
        std::cout << g;
        return 0;
}
