CXX := g++
CXXFLAGS := -pedantic -Wall -std=c++20
LD := g++
LDFLAGS := $(CXXFLAGS)

sources := main.cpp tiles.cpp grid.cpp

all: main.out

main.out: $(sources:.cpp=.o)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm $(sources:.cpp=.o) main.out
