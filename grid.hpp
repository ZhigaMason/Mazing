#ifndef GRID_HPP_12837891273891278
#define GRID_HPP_12837891273891278
#include "tiles.hpp"
#include <set>
#include <vector>

struct Grid {
        struct Coords {
                int x, y;
                friend bool operator==(const Coords & l, const Coords & r);
                friend auto operator<=>(const Coords & l, const Coords & r);
        };

        Grid(int height, int width, Coords start, Coords finish);
        Grid(int height, int width);
        ~Grid() noexcept;
        // Rule of 3, can be changed later if copy constructor needed, to lazy to implement deep copy
        Grid(const Grid &) = delete;
        Grid & operator=(const Grid &) = delete;

        friend std::ostream & operator<<(std::ostream&, const Grid &);

        void fill_grid(long seed = 0);

        const Coords start, finish;
private:
        TILE ** m_data;
        const int m_height, m_width;

        enum class DIRECTION : char {UP = 1, DOWN = -1, LEFT = 2, RIGHT = -2};

        std::vector<Grid::Coords> neighbors(Coords v) const;
        //Coords step(Coords, DIRECTION);
        Coords next_tile(Coords, DIRECTION, unsigned, std::set<Coords>);
        uint8_t ** generate_path() const;
        void colapse_wave_function(uint8_t **);
};

#endif //GRID_HPP_12837891273891278
