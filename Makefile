CXX := clang++
CXXFLAGS := -pedantic -Wall -std=c++20
LD := clang++
LDFLAGS := $(CXXFLAGS)

sources := cli/main.cpp src/tile/tile.cpp src/grid/grid.cpp
targets := genlab

all: $(targets)

genlab: $(sources:.cpp=.o)
	$(LD) $(LDFLAGS) $^ -o $@

rules:
	@$(CXX) -MM $(sources)

clean:
	rm $(sources:.cpp=.o)

clean-all: clean
	rm $(targets)

include dependencies.mk
