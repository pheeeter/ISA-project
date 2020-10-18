CC=g++
SRC=src/dns.cpp
BIN=dns
FLAGS= -std=c++11 -Wall -Wextra

all: $(BIN)
	
$(BIN): $(SRC)
	$(CC) $(SRC) $(FLAGS) -o $(BIN)

test:
	$(CC) $(SRC) $(FLAGS) -o $(BIN)

clean:
	rm $(BIN)