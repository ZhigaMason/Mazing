CC := clang
CFLAGS := -pedantic -Wall -O2 -fPIC
LD := clang
LDFLAGS := $(CFLAGS)
PYTHON := python3

sources := cli/main.c src/tile/tile.c src/grid/grid.c \
	   src/grid/data_structures/stack.c src/grid/data_structures/random_container.c

python_extension := python_ext
executable := genlab $(python_extension)

all: $(executable)

genlab: $(sources:.c=.o)
	$(LD) $(LDFLAGS) $^ -o $@

$(python_extension): setup.py
	$(PYTHON) $< build
	$(PYTHON) -m pip install .

clean:
	rm $(sources:.c=.o)

clean-python:
	rm -rf build dist *.egg-info

clean-all: clean
	rm $(executable)
