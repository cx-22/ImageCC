CC = gcc
CFLAGS = -g -O2 -Iinclude

SRC = $(wildcard src/*.c)
OUT = test

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f *.o test