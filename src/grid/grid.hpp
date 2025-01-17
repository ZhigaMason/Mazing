#ifndef GRID_HPP_12837891273891278
#define GRID_HPP_12837891273891278
#include "../tile/tile.hpp"
#include <vector>
#include <memory>

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

        TILE ** data();
        const TILE *const * data() const;

        const Coords start, finish;
        const int height, width;
private:
        using matrix_u8 = std::unique_ptr<std::unique_ptr<uint8_t[]>[]>;
        TILE ** m_data;

        static std::pair<TILE, TILE> tiles_from_step(Grid::Coords src, Grid::Coords dst);
        std::vector<Grid::Coords> neighbors(Coords v) const;
        matrix_u8 blank_probability() const;
        matrix_u8 generate_path() const;
        void colapse_wave_function(matrix_u8);
};

#endif //GRID_HPP_12837891273891278
