/*  pngblur.c - small tool for bluring png images
	Copyright (c) 2014 Jakub Sokol

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.  

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.

	Jakub Sokol
	beamerkun@gmail.com
 */

#include <stdlib.h>
#include <stdio.h>
#include "pnglite.h"
#define BYTES_PER_PIXEL 3

typedef unsigned char byte;

void blur(byte* source, byte* result, png_t* info, int ksize)
{
	unsigned int *temp;
	int px;

	temp = malloc(sizeof(unsigned int) * BYTES_PER_PIXEL * info->width * info->height ); 

	for( int y = 0; y < info->height; ++y )
	{
		for( int x = 0; x < info->width; ++x )
		{
			px = BYTES_PER_PIXEL * (x + info->width * y);
			for( int i = 0; i < BYTES_PER_PIXEL; ++i )
			{
				temp[px + i] = source[px + i];
				if( x != 0 )
					temp[px + i] += temp[ px - BYTES_PER_PIXEL + i];
				if( y != 0 )
					temp[px + i] += temp[ px - BYTES_PER_PIXEL * info->width + i];
				if( x != 0 && y != 0 )
					temp[px + i] -= temp[ px - BYTES_PER_PIXEL * (info->width + 1) + i ];
			}
		}
	}

	double filter = (double) 1 / (4*ksize*ksize);
	int t;

	for( int y = 0; y < info->height; ++y )
	{
		for( int x = 0; x < info->width; ++x )
		{
			int x_min, x_max, y_min, y_max;

			if( (x_min = x - ksize) < 0 )
				x_min = 0;
			if( (x_max = x + ksize) > info->width-1 )
				x_max = info->width-1;
			if( (y_min = y - ksize) < 0 )
				y_min = 0;
			if( (y_max = y + ksize) > info->height-1 )
				y_max = info->height-1;

			px = BYTES_PER_PIXEL * (x + info->width * y);

			for( int i = 0; i < BYTES_PER_PIXEL; ++i)
			{
				t =		temp[ BYTES_PER_PIXEL * (x_min + info->width * y_min)	+ i ];
				t +=	temp[ BYTES_PER_PIXEL * (x_max + info->width * y_max)	+ i ];
				t -=	temp[ BYTES_PER_PIXEL * (x_max + info->width * y_min)	+ i ];
				t -=	temp[ BYTES_PER_PIXEL * (x_min + info->width * y_max)	+ i ];
				t *=	filter;
				result[ px + i ] = t;
			}
		}
	}

	free(temp);
}

int main (int argc, char** argv)
{
	png_t info, dst;
	byte *file, *result;
	char *src_f, *dst_f;
	int ksize;

	src_f = argc > 1 ? argv[1] : NULL;
	dst_f = argc > 2 ? argv[2] : NULL;
	ksize = argc > 3 ? atoi(argv[3]) : 3;

	if( src_f == NULL || dst_f == NULL )
		exit(1);

	png_init(0, 0);
	png_open_file(&info, src_f);

	file = malloc(BYTES_PER_PIXEL * sizeof(byte) * info.width * info.height);
	result = malloc(BYTES_PER_PIXEL * sizeof(byte) * info.width * info.height);

	png_get_data(&info, file);
	png_close_file(&info);

	blur(file, result, &info, ksize);
	blur(result, file, &info, ksize);
	blur(file, result, &info, ksize);

	png_open_file_write(&dst, dst_f);
	png_set_data(&dst, info.width, info.height, info.depth, info.color_type, result);
	png_close_file(&dst);

	free(file);
	free(result);

	return 0;
}
