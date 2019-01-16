## YAP Makefile

CC = g++
SRC = src/main.cpp
BIN = yap

all:
	@echo "Building YAP"
	@$(CC) $(SRC) -o ${BIN}

clean:
	@rm yap
