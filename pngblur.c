/*  pngblur.h - Interface for pnglite library
	Copyright (c) 2014 Daniel Karling

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

typedef unsigned char byte;

// TODO variable kernel size
void genAddr( int* addr, int x, int y, png_t* info )
{
	int px, width_px;
	px = 3 * x + 3 * info->width * y;
	width_px = 3 * info->width;

	for( int i = 0; i < 9; ++i )
		addr[i] = px;

	if( y != 0 )
	{
		if( x != 0 )
			addr[0] = px - 3 - width_px;
		addr[1] = px - width_px;
		if( x != info->width - 1 )
			addr[2] = px + 3 - width_px;
	}

	if( x != 0 )
		addr[3] = px - 3;
	addr[4] = px;
	if( x != info->width - 1 )
		addr[5] = px + 3;

	if( y != info->height - 1 )
	{
		if( x != 0 )
			addr[3] = px - 3 + width_px;
		addr[4] = px + width_px;
		if( x != info->width - 1 )
			addr[5] = px + 3 + width_px;
	}
}

// TODO variable kernel size
void blur(byte* source, byte* result, png_t* info)
{
	float filter[] =
	{
		0.125,	0.125,	0.125,
		0.125,	0.125,	0.125,
		0.125,	0.125,	0.125
	};

	for( int y = 0; y < info->height; ++y )
	{
		for( int x = 0; x < info->width; ++x )
		{
			int px;
			int addr[9];

			genAddr(addr, x, y, info);

			px = addr[4];

			for( int i = 0; i < 3; ++i )
			{
				for( int j = 0; j < 9; ++j )
					result[px + i] += source[ addr[j] + i ] * filter[j];
			}
		}
	}
}

int main (int argc, char** argv)
{
	png_t info, dst;
	byte *file, *result;
	char *src_f, *dst_f;

	src_f = argc >= 1 ? argv[1] : NULL;
	dst_f = argc >= 2 ? argv[2] : NULL;

	if( src_f == NULL || dst_f == NULL )
		exit(1);

	png_init(0, 0);
	png_open_file(&info, src_f);

	file = malloc(4 * sizeof(byte) * info.width * info.height);
	result = malloc(4 * sizeof(byte) * info.width * info.height);

	png_get_data(&info, file);
	png_close_file(&info);

	blur(file, result, &info);

	png_open_file_write(&dst, dst_f);
	png_set_data(&dst, info.width, info.height, info.depth, info.color_type, result);

	free(file);
	free(result);

	return 0;
}
