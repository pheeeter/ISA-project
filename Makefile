CC=g++
SRC=src/*.cpp
BIN=dns
FLAGS= -std=c++11 -Wall -Wextra
PACK=xkoprd00

all: $(BIN)
	
$(BIN): $(SRC)
	$(CC) $(SRC) $(FLAGS) -o $(BIN)

test:
	@$(CC) $(SRC) $(FLAGS) -o $(BIN)
	@chmod +x tests/script.sh
	@tests/script.sh

pack:
	tar -cvf $(PACK).tar src/* tests/* Makefile README manual.pdf

clean:
	rm $(BIN)