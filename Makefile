## YAP Makefile

CC = g++
CFLAGS = -lstdc++fs -std=c++17

SRC = src/main.cpp
BIN = yap

all:
	@echo "building YAP"
	@$(CC) $(SRC) -o ${BIN} ${CFLAGS}

clean:
	@rm yap
