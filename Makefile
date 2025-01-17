CC := clang
CFLAGS := -pedantic -Wall -std=c23
LD := clang
LDFLAGS := $(CFLAGS)

sources := cli/main.c src/tile/tile.c
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
