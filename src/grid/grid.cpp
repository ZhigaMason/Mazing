#ifndef GRID_CPP_123927891273812731293719
#define GRID_CPP_123927891273812731293719
#include "grid.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <array>
#include <set>
#include <queue>

bool operator==(const Grid::Coords & l, const Grid::Coords & r) = default;
auto operator<=>(const Grid::Coords & l, const Grid::Coords & r) = default;

Grid::Grid(int height, int width, Coords start, Coords finish) :
        start(start), finish(finish), height(height), width(width) {

        if(width < 1 || height < 1)
                throw std::invalid_argument("Invalid width or height");

        if (start.x >= width || start.y >= height || finish.x >= width || finish.y >= height)
                throw std::invalid_argument("Finish and start positions must be inside of grid");

        m_data = new TILE*[height];
        TILE ** end = m_data + height;
        for(TILE ** it = m_data; it != end; ++it) {
                *it = new TILE[width];
                TILE * jend = *it + width;
                for(TILE * jt = *it; jt != jend; ++jt) *jt = TILE::WALL;
        }
}
Grid::Grid(int height, int width)
    : Grid(height, width, Coords(0, 0), Coords(height - 1, width - 1)) {}

Grid::~Grid() noexcept {
        TILE ** end = m_data + height;
        for(TILE ** it = m_data; it != end; ++it) {
                delete [] *it;
        }
        delete [] m_data;
}

std::ostream & operator<<(std::ostream & os, const Grid & grid) {

        TILE ** end = grid.m_data + grid.height;
        for(TILE ** it = grid.m_data; it != end; ++it) {
                std::string line = tileio::line_to_string(*it, grid.width);
                int y = it - grid.m_data;
                if(y == grid.start.y) line[(grid.start.x + grid.width) * 3 + 2] = 'S';
                if(y == grid.finish.y) line[(grid.finish.x + grid.width) * 3 + 2] = 'E';
                os << line;
        }
        return os;
}

void Grid::fill_grid(long seed) {
        srand(seed);
        colapse_wave_function(generate_path());
}

TILE ** Grid::data() { return m_data; }
const TILE *const * Grid::data() const {return m_data;}

std::pair<TILE, TILE> Grid::tiles_from_step(Grid::Coords src, Grid::Coords dst) {
        int dx = src.x - dst.x, dy = src.y - dst.y;
        TILE ret_src = TILE::WALL;
        TILE ret_dst = TILE::WALL;
        if(dx == 1) {
                ret_src |= TILE::LEFT;
                ret_dst |= TILE::RIGHT;
        }
        else if(dx == -1) {
                ret_src |= TILE::RIGHT;
                ret_dst |= TILE::LEFT;
        }

        if(dy == 1) {
                ret_src |= TILE::UP;
                ret_dst |= TILE::DOWN;
        }
        else if(dy == -1) {
                ret_src |= TILE::DOWN;
                ret_dst |= TILE::UP;
        }

        return {ret_src, ret_dst};
}

std::vector<Grid::Coords> Grid::neighbors(Coords v) const {
        std::vector<Coords> ret;
        ret.reserve(4);
        if(v.x > 0) ret.emplace_back(v.x - 1, v.y);
        if(v.y > 0) ret.emplace_back(v.x, v.y - 1);
        if(v.x < (width - 1))  ret.emplace_back(v.x + 1, v.y);
        if(v.y < (height - 1)) ret.emplace_back(v.x, v.y + 1);
        return ret;
}

Grid::matrix_u8 Grid::blank_probability() const {
        matrix_u8 proba_space(new std::unique_ptr<uint8_t[]>[height]);

        for(int i = 0; i != height; ++i) {
                proba_space[i] = std::unique_ptr<uint8_t[]>(new uint8_t[width]);
                uint8_t* end = proba_space[i].get() + width;
                for (uint8_t *it = proba_space[i].get(); it != end; ++it)
                  *it = 0b1111;
        }

        for(int i = 0; i < height; ++i) {
                proba_space[i][0] &= ~static_cast<uint8_t>(TILE::LEFT);
                proba_space[i][width - 1] &= ~static_cast<uint8_t>(TILE::RIGHT);
        }

        for(int i = 0; i < width; ++i) {
                proba_space[0][i] &= ~static_cast<uint8_t>(TILE::UP);
                proba_space[height - 1][i] &= ~static_cast<uint8_t>(TILE::DOWN);
        }

        return proba_space;
}

Grid::matrix_u8 Grid::generate_path() const {

        std::set<Coords> visited{start};
        std::deque<Coords> st;
        st.push_back(start);

        while(!st.empty() && st.back() != finish) {
                Coords curr = st.back();

                bool foundnew = false;
                auto adj = neighbors(curr);
                while(!adj.empty()){
                        unsigned char idx = rand() % adj.size();
                        Coords n = adj[idx];
                        if(visited.contains(n)) {
                                adj.erase(adj.begin() + idx);
                                continue;
                        }
                        visited.insert(n);
                        st.push_back(n);
                        foundnew = true;
                        break;
                }

                if(!foundnew) st.pop_back();
        }

        matrix_u8 prob_space = blank_probability();

        for (auto it = st.rbegin(); (it + 1) != st.rend(); ++it) {
                auto pit = it + 1;
                auto [src_tile, dst_tile] = tiles_from_step(*pit, *it);
                m_data[pit->y][pit->x] |= src_tile;
                m_data[it->y][it->x]   |= dst_tile;
                prob_space[pit->y][pit->x] &= ~static_cast<uint8_t>(src_tile);
                prob_space[it->y][it->x]   &= ~static_cast<uint8_t>(dst_tile);
        }

        return prob_space;
}

void Grid::colapse_wave_function(Grid::matrix_u8 proba_space) {
        struct PrCoords {
                PrCoords(Coords c) : c(c), p(rand()) {}
                Coords c;
                size_t p;
                bool operator<(const PrCoords & other) const {return p < other.p;}
        };
        std::set<Coords> visited{start, finish};
        std::priority_queue<PrCoords> open;
        open.emplace(start); open.emplace(finish);

        while(!open.empty()) {
                auto [curr, _] = open.top(); open.pop();
                m_data[curr.y][curr.x] |= TILE(uint8_t(rand()) & proba_space[curr.y][curr.x]); // Generating the tile
                TILE curr_tile = m_data[curr.y][curr.x];

                for (auto [neighbor, tile] : std::array<std::pair<Coords, TILE>, 4>{
                         std::pair{Coords(curr.x - 1, curr.y), TILE::LEFT},
                         std::pair{Coords(curr.x, curr.y - 1), TILE::UP},
                         std::pair{Coords(curr.x + 1, curr.y), TILE::RIGHT},
                         std::pair{Coords(curr.x, curr.y + 1), TILE::DOWN},
                }) {
                        if(neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= width || neighbor.y >= height) continue;
                        TILE new_tile = curr_tile & inverse_tile(tile);
                        m_data[neighbor.y][neighbor.x] |= new_tile;
                        proba_space[neighbor.y][neighbor.x] &= ~static_cast<uint8_t>(tile);
                        if (!visited.contains(neighbor)) {
                          visited.insert(neighbor);
                          open.emplace(neighbor);
                        }
                }
        }
}

#endif  //GRID_CPP_123927891273812731293719
