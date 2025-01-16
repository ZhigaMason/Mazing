CXX := g++
CXXFLAGS := -pedantic -Wall -std=c++20
LD := g++

sources := main.cpp tiles.cpp grid.cpp

all: main.out

main.out: $(sources:.cpp=.o)
	$(LD) $^ -o $@

clean:
	rm $(sources:.cpp=.o) main.out
