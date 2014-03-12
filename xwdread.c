/*
   xwdread.c - tool for reading xwd dumps
   
   Copyright (c) 2014 Jakub Sokol

   full license in xwdread.h
*/

#include "xwdread.h"

byte* readXWD( char* filename, int* width, int* height )
{
	FILE* file;
	byte* source;

	if( strcmp("-",filename) == 0 )
	{
		file = stdin;
	}
	else
	{
		file = fopen(filename, "rb");
	}

	XWDFileHeader xwd;
	fread( &xwd, sizeof(XWDFileHeader), 1, file );


	*width = swab(xwd.pixmap_width);
	*height = swab(xwd.pixmap_height);
	int px_count = swab(xwd.pixmap_width) * swab(xwd.pixmap_height);

	source = malloc( BYTES_PER_PIXEL * px_count );
	XWDPixel* pixmap = malloc( sizeof(XWDPixel) * px_count );
	fread(pixmap, swab(xwd.header_size)-sizeof(XWDFileHeader)+swab(xwd.ncolors)*sizeof(XWDColor), 1, file);
	fread(pixmap, sizeof(XWDPixel)*px_count, 1, file);
	fclose(file);

	for(int i = 0; i < px_count; ++i )
	{
		XWDPixel px = pixmap[i];
		source[BYTES_PER_PIXEL * i] = px.red;
		source[BYTES_PER_PIXEL * i + 1] = px.green;
		source[BYTES_PER_PIXEL * i + 2] = px.blue;
	}

	free(pixmap);
	return source;
}
