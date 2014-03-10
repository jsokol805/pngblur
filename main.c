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

	argc >= 1 ? src_f = argv[1] : exit(1);
	argc >= 2 ? dst_f = argv[2] : exit(1);

	png_init(0, 0);
	png_open_file(&info, "test.png");

	file = malloc(4 * sizeof(byte) * info.width * info.height);
	result = malloc(4 * sizeof(byte) * info.width * info.height);

	png_get_data(&info, file);
	png_close_file(&info);

	blur(file, result, &info);

	png_open_file_write(&dst, "blur.png");
	png_set_data(&dst, info.width, info.height, info.depth, info.color_type, result);

	free(file);
	free(result);

	return 0;
}
