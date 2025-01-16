#ifndef GRID_CPP_123927891273812731293719
#define GRID_CPP_123927891273812731293719
#include "grid.hpp"
#include "tiles.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <queue>

void shout(std::string s) { std::cout << s << std::endl; }

bool operator==(const Grid::Coords & l, const Grid::Coords & r) = default;
auto operator<=>(const Grid::Coords & l, const Grid::Coords & r) = default;

Grid::Grid(int height, int width, Coords start, Coords finish) :
        start(start), finish(finish), m_height(height), m_width(width) {

        if(m_width < 1 || m_height < 1)
                throw std::invalid_argument("Invalid width or height");

        if (start.x >= m_width || start.y >= m_height || finish.x >= m_width || finish.y >= m_height)
                throw std::invalid_argument("Finish and start positions must be inside of grid");

        m_data = new TILE*[m_height];
        TILE ** end = m_data + m_height;
        for(TILE ** it = m_data; it != end; ++it) {
                *it = new TILE[m_width];
                TILE * jend = *it + m_width;
                for(TILE * jt = *it; jt != jend; ++jt) *jt = TILE::WALL;
        }
}
Grid::Grid(int height, int width)
    : Grid(height, width, Coords(0, 0), Coords(height - 1, width - 1)) {}

Grid::~Grid() noexcept {
        TILE ** end = m_data + m_height;
        for(TILE ** it = m_data; it != end; ++it) {
                delete [] *it;
        }
        delete m_data;
}

std::ostream & operator<<(std::ostream & os, const Grid & grid) {

        TILE ** end = grid.m_data + grid.m_height;
        for(TILE ** it = grid.m_data; it != end; ++it) {
                std::string line = line_to_string(*it, grid.m_width);
                int y = it - grid.m_data;
                if(y == grid.start.y) line[(grid.start.x + grid.m_width) * 3 + 2] = 'S';
                if(y == grid.finish.y) line[(grid.finish.x + grid.m_width) * 3 + 2] = 'E';
                os << line;
        }
        return os;
}

void Grid::fill_grid(long seed) {
        srand(seed);
        colapse_wave_function(generate_path());
}

std::pair<TILE, TILE> tiles_from_step(Grid::Coords src, Grid::Coords dst) {
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
        if(v.x < (m_width - 1))  ret.emplace_back(v.x + 1, v.y);
        if(v.y < (m_height - 1)) ret.emplace_back(v.x, v.y + 1);
        return ret;
}

Grid::Coords Grid::next_tile(Grid::Coords curr, Grid::DIRECTION prev_d,
                             unsigned n_same_d,
                             std::set<Grid::Coords> visited) {
  return Coords(-1, -1);
}

uint8_t ** Grid::generate_path() const {

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

        uint8_t ** prob_space = new uint8_t*[m_height];

        uint8_t ** end = prob_space + m_height;
        for(uint8_t ** it = prob_space; it != end; ++it) {
                *it = new uint8_t[m_width];
                uint8_t* jend = *it + m_width;
                for (uint8_t *jt = *it; jt != jend; ++jt)
                  *jt = 0b1111;
        }

        for(int i = 0; i < m_height; ++i) {
                prob_space[i][0] &= 0;//~static_cast<uint8_t>(TILE::LEFT);
                prob_space[i][m_width - 1] &= 0;//~static_cast<uint8_t>(TILE::RIGHT);
        }

        for(int i = 0; i < m_width; ++i) {
                prob_space[0][i] &= 0;//~static_cast<uint8_t>(TILE::UP);
                prob_space[m_height - 1][i] &= 0;//~static_cast<uint8_t>(TILE::DOWN);
        }

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

void Grid::colapse_wave_function(uint8_t ** proba_space) {
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

                for (auto [neighbor, tile] : std::vector<std::pair<Coords, TILE>>{
                         {Coords(curr.x - 1, curr.y), TILE::LEFT},
                         {Coords(curr.x, curr.y - 1), TILE::UP},
                         {Coords(curr.x + 1, curr.y), TILE::RIGHT},
                         {Coords(curr.x, curr.y + 1), TILE::DOWN},
                }) {
                        if(neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= m_width || neighbor.y >= m_height) continue;
                        TILE new_tile = curr_tile & inverse_tile(tile);
                        m_data[neighbor.y][neighbor.x] |= new_tile;
                        proba_space[neighbor.y][neighbor.x] &= ~static_cast<uint8_t>(tile);
                        if (!visited.contains(neighbor)) {
                          visited.insert(neighbor);
                          open.emplace(neighbor);
                        }
                }
        }

        for(int i = 0; i < m_height; ++i) {
                delete [] proba_space[i];
        }
        delete [] proba_space;
}

#endif  //GRID_CPP_123927891273812731293719
