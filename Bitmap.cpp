////////////////////////////////////////
// Bitmap.cpp
////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "Bitmap.h"

////////////////////////////////////////////////////////////////////////////////

struct BitmapHeader {
	int FileSize;
	int Reserved;
	int StartAddress;
	int HeaderSize;
	short Width;
	short Height;
	short ColorPlanes;
	short BitsPerPixel;
};

////////////////////////////////////////////////////////////////////////////////

bool Bitmap::SaveBMP(const char *filename) {
	BitmapHeader head;
	head.FileSize=sizeof(BitmapHeader)+2+XRes*YRes*sizeof(int);
	head.Reserved=0;
	head.StartAddress=sizeof(BitmapHeader)+2;

	head.HeaderSize=12;
	head.Width=(short)XRes;
	head.Height=(short)YRes;
	head.ColorPlanes=1;
	head.BitsPerPixel=32;

    FILE *f = fopen(filename, "wb");
	if(f==0) {
		printf("Can't open '%s'\n",filename);
		return false;
	}
	short magic=19778;
	fwrite(&magic,sizeof(short),1,f);
	fwrite(&head,sizeof(BitmapHeader),1,f);
	fwrite(Pixel,sizeof(int),XRes*YRes,f);
	fclose(f);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
