/*  xwdread.h - routine for reading xwd dumps
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

#ifndef __JS_XWDREAD
#define __JS_XWDREAD

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/XWDFile.h>
#include "pngblur.h"
#define swab __builtin_bswap32

typedef struct {
	byte blue;
	byte green;
	byte red;
	byte pad;
} XWDPixel;

byte* readXWD( char* filename, int* width, int* height );
#endif
