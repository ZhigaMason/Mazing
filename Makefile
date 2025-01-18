CC := clang
CFLAGS := -pedantic -Wall -O2 -fPIC -std=c23
LD := clang
LDFLAGS := $(CFLAGS)

sources := cli/main.c src/tile/tile.c src/grid/grid.c \
	   src/grid/data_structures/stack.c src/grid/data_structures/random_container.c
targets := genlab

all: $(targets)

genlab: $(sources:.c=.o)
	$(LD) $(LDFLAGS) $^ -o $@

rules:
	@$(CXX) -MM $(sources)

clean:
	rm $(sources:.c=.o)

clean-all: clean
	rm $(targets)
	rm $(sources:.c=.o)

include dependencies.mk
