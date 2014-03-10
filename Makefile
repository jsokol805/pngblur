CFLAGS = -std=c99 -Wall -pedantic -Werror
LDFLAGS = -lz
CC = gcc
FILES = pngblur.o pnglite.o

.c.o:
	$(CC) $(CFLAGS) -c $<

pngblur: $(FILES)
	$(CC) $(LDFLAGS) $(FILES) -o $@

all: pngblur

