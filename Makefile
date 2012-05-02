CC ?= gcc
CFLAGS = -Wall
PROGRAM = bfi
SOURCE = bfi.c

all: bfi.c
	$(CC) $(SOURCE) $(CFLAGS) -o $(PROGRAM)

clean:
	@rm -rf $(PROGRAM)
