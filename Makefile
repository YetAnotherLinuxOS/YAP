## YAP Makefile

CC = g++
CXXFLAGS = -lstdc++fs -std=c++17
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = yap

all: $(BIN)

yap: $(OBJ)
	$(CC) $^ -o $@ $(CXXFLAGS)

yap.o: src/yap.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

main.o: src/main.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm yap
	rm src/*.o
