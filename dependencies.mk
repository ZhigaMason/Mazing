main.o: cli/main.c cli/../src/grid/grid.h cli/../src/grid/../tile/tile.h
tile.o: src/tile/tile.c src/tile/tile.h
grid.o: src/grid/grid.c src/grid/grid.h src/grid/../tile/tile.h \
 src/grid/data_structures/stack.h src/grid/data_structures/../grid.h \
 src/grid/data_structures/random_container.h
stack.o: src/grid/data_structures/stack.c \
 src/grid/data_structures/stack.h src/grid/data_structures/../grid.h \
 src/grid/data_structures/../../tile/tile.h
random_container.o: src/grid/data_structures/random_container.c \
 src/grid/data_structures/random_container.h \
 src/grid/data_structures/../grid.h \
 src/grid/data_structures/../../tile/tile.h
