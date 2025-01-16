CXX := g++
CXXFLAGS := -pedantic -Wall
LD := g++

sources := main.cpp tiles.cpp

all: main.out

main.out: $(sources:.cpp=.o)
	$(LD) $^ -o $@

clean:
	rm $(sources:.cpp=.o) main.out
