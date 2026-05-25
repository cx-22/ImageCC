CC = emcc

CFLAGS = -O2 -g -Iinclude

SRC = $(wildcard src/*.c)
OUT = test.html

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) \
	-s WASM=1 \
	-s MODULARIZE=1 \
	-s EXPORT_ES6=1 \
	-s EXPORTED_RUNTIME_METHODS="['cwrap','ccall']"

clean:
	rm -f test.html test.js test.wasm