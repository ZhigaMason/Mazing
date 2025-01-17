main.o: cli/main.cpp cli/../src/grid/grid.hpp \
  cli/../src/grid/../tile/tile.hpp
tile.o: src/tile/tile.cpp src/tile/tile.hpp
grid.o: src/grid/grid.cpp src/grid/grid.hpp src/grid/../tile/tile.hpp
