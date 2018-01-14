CFLAGS=		-std=c99 -pedantic -Wall -Werror -Wextra -g
SRC=		token.c test.c
BIN=		test

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

.PHONY: clean

clean:
	rm -f $(BIN)
