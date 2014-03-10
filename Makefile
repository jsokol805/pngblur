all:
	gcc -lz -std=c99 -Wall main.c pnglite.c -lm -o blur

