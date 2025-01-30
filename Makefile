CC := clang
CFLAGS := -pedantic -Wall -O2 -fPIC
LD := clang
LDFLAGS := $(CFLAGS)
PYTHON := python3

sources := cli/main.c src/tile/tile.c src/grid/grid.c \
	   src/grid/data_structures/stack.c src/grid/data_structures/random_container.c

python_extension := build/lib.linux-x86_64-cpython-312/labyrinth_generator.cpython-312-x86_64-linux-gnu.so
targets := genlab $(python_extension)

all: $(targets)

genlab: $(sources:.c=.o)
	$(LD) $(LDFLAGS) $^ -o $@

$(python_extension): setup.py
	python3 setup.py build

rules:
	@$(CXX) -MM $(sources)

clean:
	rm $(sources:.c=.o)

clean-all: clean
	rm $(targets)
	rm $(sources:.c=.o)

include Makefile.d
