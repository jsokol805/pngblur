CFLAGS = -g -std=c99 -O2 -Wall -pedantic -fstack-protector -Wl,-z,relro -Wl,-z,now -Wformat-security -Werror
LDFLAGS = -lz -Wl,-O1 -Wl,--build-id=sha1 -Wl,--no-undefined
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
