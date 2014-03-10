CFLAGS = -std=c99 -Wall -pedantic -Werror
LDFLAGS = -lz
CC = gcc
FILES = pngblur.o pnglite.o
PROG = pngblur

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROG): $(FILES)
	$(CC) $(LDFLAGS) $(FILES) -o $@

all: $(PROG)

clean:
	rm -rf $(FILES) $(PROG)
