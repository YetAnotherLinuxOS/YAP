## YAP Makefile

CXX = clang++
CXXFLAGS = -lstdc++fs -std=c++17
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = yap

all: $(BIN)

yap: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

yap.o: src/yap.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

main.o: src/main.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm yap
	rm src/*.o
