## YAP Makefile

CXX = clang++
CXXFLAGS = -lstdc++fs -std=c++17 -Wall
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = yap

all: $(BIN)

yap: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm src/*.o || echo
	rm yap || echo
